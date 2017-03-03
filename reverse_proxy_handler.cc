#include <boost/algorithm/string/replace.hpp>
#include <boost/asio.hpp>
#include <boost/tokenizer.hpp>
#include <iostream>
#include "reverse_proxy_handler.h"

using boost::asio::ip::tcp;

// Helper function for tokenizing HTTP requests
// From: https://github.com/UCLA-CS130/Mr.-Robot-et-al./blob/c9b064c68cd4bc1ae6b5c012db59eae9cb8b946d/request.cc#L7
boost::tokenizer<boost::char_separator<char>>
tokenGenerator(const std::string s, const char* sep) {
    // Tokenize the rest_lines
    boost::char_separator<char> separator(sep);
    boost::tokenizer<boost::char_separator<char>> tokens(s, separator);
    return tokens;
}

// Initializes the handler. Returns OK if successful
// uri_prefix is the value in the config file that this handler will run for
// config is the contents of the child block for this handler ONLY
RequestHandler::Status ReverseProxyHandler::Init(const std::string& uri_prefix,
    const NginxConfig& config) {
    // Check config child block for settings
    for (size_t i = 0; i < config.statements.size(); i++) {
        // Only attempt to read statements with a first token we care about
        if (config.statements[i]->tokens.size() > 0) {
            if (config.statements[i]->tokens[0] == "remote_host") {
              // remote_host must have 2 tokens
              if (config.statements[i]->tokens.size() > 1) {
                  remote_host = config.statements[i]->tokens[1];
              } else {
                  printf("\"remote_host\" statement in config needs more tokens\n");
              }
          }
          if (config.statements[i]->tokens[0] == "remote_port") {
              // remote_port must have 2 tokens
              if (config.statements[i]->tokens.size() > 1) {
                  remote_port = config.statements[i]->tokens[1];
              } else {
                  printf("\"remote_port\" statement in config needs more tokens\n");
              }
          }
      }
    }
    if (!remote_host.empty() && !remote_port.empty()) {
        original_uri_prefix = uri_prefix;
        return RequestHandler::OK;
    } else {
        printf("Missing remote_port or remote_host provided to ReverseProxyHandler\n");
        return RequestHandler::Error;
    }
}

// Helper for 302-redirects
// Returns the value of the Location header,
// e.g., "www.exmple.com" for "Location: www.example.com"
std::string ReverseProxyHandler::getLocationHeaderValue(const std::string& response_buffer_string) {
    // Grab the Location header line
    int start_location = response_buffer_string.find("Location:");
    int end_location = response_buffer_string.find_first_of("\r\n", start_location);
    std::string location_line = response_buffer_string.substr(start_location, end_location);

    boost::tokenizer<boost::char_separator<char>> tokens
        = tokenGenerator(location_line, " ");

    // Split the location line to get the value
    std::string location_value;
    int i = 0;
    for (auto cur_token = tokens.begin();
         cur_token != tokens.end();
         cur_token++,
         i++) {
        if (i == 1) {
            location_value = *cur_token;
            break;
        }
    }

    return location_value;
}

// Helper for parsing response code from origin responses
std::string ReverseProxyHandler::getRemoteResponseCode(const std::string& response_buffer_string) {
    // Response has the form:
    // HTTP/1.0 200 OK
    // Content-Length: 3497
    // Content-Type: text/plain
    //
    // <body contents>
    //
    // We check just the first line
    size_t end_first_line = response_buffer_string.find_first_of("\r\n");
    std::string first_line = response_buffer_string.substr(0, end_first_line);

    // TODO: Move this response-code checking into a helper function

    // Tokenize and check second token for ResponseCode
    // Based on: https://github.com/UCLA-CS130/Mr.-Robot-et-al./blob/c9b064c68cd4bc1ae6b5c012db59eae9cb8b946d/request.cc#L46
    boost::tokenizer<boost::char_separator<char>> tokens
        = tokenGenerator(first_line, " ");

    // Handle response code from remote_host.
    // remote_host's ResponseCode => our ResponseCode cases:
    //
    // 200 => 200
    // 302 => fetch-loop to 200 or 404 (upon not-found or max-retries)
    // 404 => 404
    // 500 => 404
    std::string return_response_code;
    int i = 0;
    for (auto cur_token = tokens.begin();
         cur_token != tokens.end();
         cur_token++,
         i++) {
        if (i == 1) {
            std::string remote_response_code = *cur_token;
            std::cerr << "remote_response_code: " << remote_response_code << std::endl;

            if (remote_response_code == "200") {
                return_response_code = "200";
            } else if (remote_response_code == "302") {
                return_response_code = "302";
            } else if (remote_response_code == "404") {
                return_response_code = "404";
            } else if (remote_response_code == "500") {
                return_response_code = "404";
            } else {
                printf("ReverseProxyHandler does not understand the remote_host's ResponseCode\n");
                return_response_code = "500";
            }
            break;
        }
    }

    return return_response_code;
}

// Helper for sending proxy requests
// Returns the string representation of the response received from the remote_host
// Origin is the remote_host that is the original source of the content
// we're serving on their behalf
std::string ReverseProxyHandler::sendRequestToOrigin(Request request, std::string remote_uri) {
    boost::asio::io_service io_service;
    tcp::socket socket(io_service);
    tcp::resolver resolver(io_service);
    // Avoid "host not found" error
    // See: https://stackoverflow.com/questions/12542460/boost-asio-host-not-found-authorative
    tcp::resolver::query query(remote_host,
                               remote_port,
                               boost::asio::ip::resolver_query_base::numeric_service);

    // Use resolver to handle DNS lookup if query is not an IP address
    boost::system::error_code ec;

    // A hostname could resolve to multiple endpoints to try;
    // connect() will try all of them
    // See: http://www.boost.org/doc/libs/1_62_0/boost/asio/connect.hpp
    boost::asio::connect(socket, resolver.resolve(query), ec);
    if (ec == boost::asio::error::not_found) {
        printf("Reverse proxy connection attempt to remote host failed. Check hostname and port number.\n");
        return "RequestHandler::Error";
    }

    std::cerr << "Got past connecting to remote_host!" << std::endl;

    // Construct new request to send to remote host
    // Example: Modify request from: /reverse_proxy/static/file1.txt
    // to:                           /static/file1.txt
    remote_uri.erase(0, remote_uri.find("/", 1));

    if (remote_uri.empty()) {
      remote_uri = "/";
    }

    request.SetUri(remote_uri);
    std::string remote_request = request.ToString();

    std::cerr << "Sending remote_request: " << remote_request << std::endl;

    // TODO: for some reason the server fails to transfer an image through the reverse_proxy.
    // Right now, it will send like 4 bytes then just sit there. Also, requests to
    // /reverse_proxy/echo and /reverse_proxy/status through curl are correctly recieved and
    // when diff'd they show nothing. However, firefox will try to download the file instead
    // of rendering text for every handler.
    boost::asio::write(socket, boost::asio::buffer(remote_request, remote_request.size()));

    // Based on: https://github.com/UCLA-CS130/Mr.-Robot-et-al./blob/c9b064c68cd4bc1ae6b5c012db59eae9cb8b946d/lightning_server.cc#L26
    const int MAX_BUFFER_LENGTH = 8192;
    char response_buffer[MAX_BUFFER_LENGTH];
    memset(response_buffer, 0, MAX_BUFFER_LENGTH);

    // TODO: need to loop and keep reading if we recieve a large file, `read_some` blocks until
    // it reads a non-zero number of bytes, but have to somehow check against its return value
    int bytes_read = socket.read_some(boost::asio::buffer(response_buffer), ec);
    std::cout << "bytes_read: " << bytes_read << std::endl;

    switch (ec.value()) {
      case boost::system::errc::success:
        std::cout << "remote_host's response: \n" << response_buffer << std::endl;
        break;
      default:
        std::cout << "Error reading from remote_host, error code: " << ec << std::endl;
        return "RequestHandler::Error";
    }

    return std::string(response_buffer);
}

// Helper for editing the path of relative URIs
// When HTML elements require resources with a path relative to their origin, for
// example a stylesheet for www.foobar.com/home which has the path:
// 'href="../css/style.css"', we need to change the '="../"' to '="/home/../"'.
void ReverseProxyHandler::rerouteRelativeUris(std::string& response_body) {
    if (original_uri_prefix != "/") {
        std::string new_href = "href=\"" + original_uri_prefix + "/";
        boost::replace_all(response_body, "href=\"/", new_href);

        std::string new_src = "src=\"" + original_uri_prefix + "/";
        boost::replace_all(response_body, "src=\"/", new_src);
    }
}

// Handles an HTTP request, and generates a response. Returns a response code
// indicating success or failure condition. If ResponseCode is not OK, the
// contents of the response object are undefined, and the server will return
// HTTP code 500
RequestHandler::Status ReverseProxyHandler::HandleRequest(const Request& request,
    Response* response) {

    std::string response_buffer_string = sendRequestToOrigin(request, request.uri());

    if (response_buffer_string == "RequestHandler::Error") {
        response->SetStatus(Response::not_found);
        return RequestHandler::Error;
    }

    std::string return_response_code = getRemoteResponseCode(response_buffer_string);
    std::cerr << "return_response_code: " << return_response_code << std::endl;

    // To avoid infinite loops, 302 redirects are followed at most 5 times
    int num_tries = 0;
    const int MAX_TRIES = 5;
    while (return_response_code == "302") {
        if (num_tries >= MAX_TRIES) {
            return_response_code = "404";
            printf("Max number of 302 Found redirects followed. Potential redirect loop.\n");
            break;
        }

        std::string new_remote_uri = getLocationHeaderValue(response_buffer_string);
        std::cout << "\nTrying to send another request with remote_url: " << new_remote_uri << std::endl << std::endl;
        // TODO: should this be setting the Host: or Location: header instead of uri?
        // for example, when redirected from ucla.edu, new_remote_uri = http://www.ucla.edu
        response_buffer_string = sendRequestToOrigin(request, new_remote_uri);
        return_response_code = getRemoteResponseCode(response_buffer_string);

        num_tries++;
    }

    if (return_response_code == "200") {
        response->SetStatus(Response::ok);
    } else if (return_response_code == "404") {
        response->SetStatus(Response::not_found);
    } else if (return_response_code == "500") {
        response->SetStatus(Response::internal_server_error);
    }

    // Construct response headers by including every line of
    // the remote_host's response after the headers
    // NOTE: find() matches an entire sequence; find_first_of() matches
    // the first of any character specified in the search pattern
    int end_headers = response_buffer_string.find("\r\n\r\n");

    std::cerr << "end_headers index: " << end_headers << std::endl;

    // Copying over headers from the remote response
    int header_start = response_buffer_string.find("\r\n") + 2;
    int header_end = response_buffer_string.find("\r\n", header_start);
    while (header_end <= end_headers) {
      // Find ':' separating header key and header value, then add two to get to the start
      // of the header value.
      int colon_pos = response_buffer_string.find_first_of(':', header_start);
      int header_type_len = colon_pos - header_start;
      int header_value_len = header_end - colon_pos - 1;
      std::string header_type = response_buffer_string.substr(header_start, header_type_len);
      std::string header_value = response_buffer_string.substr(colon_pos+2, header_value_len);
      response->AddHeader(header_type, header_value);

      // After processing current header, move position past the previously found "\r\n"
      header_start = header_end + 2;
      header_end = response_buffer_string.find("\r\n", header_start);
    }

    // Erase headers from the response after adding them to the Response object
    // the +4 is to skip the double carriage return
    response_buffer_string.erase(0, end_headers + 4);
    response->SetBody(response_buffer_string);

    return RequestHandler::OK;
}
