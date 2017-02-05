#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include "http_header.h"


namespace http {

// HTTP/1.0 response message
struct response {
    // HTTP/1.0 status codes
    enum status_code {
        ok = 200,
        created = 201,
        accepted = 202,
        no_content = 204,
        moved_permanently = 301,
        moved_temporarily = 302,
        not_modified = 304,
        bad_request = 400,
        unauthorized = 401,
        forbidden = 403,
        not_found = 404,
        internal_server_error = 500,
        not_implemented = 501,
        bad_gateway = 502,
        service_unavailable = 503
    };

    // Creates a default response for a given status code
    static response default_response(status_code status);

    // Creates a plain text response for the given text
    static response plain_text_response(std::string&& text);

    // Creates a text/html response for the given html
    static response html_response(std::string&& html);

    // Converts the response into buffers so that it can be sent to the client
    std::vector<boost::asio::const_buffer> to_buffers() const;

    status_code         status;  // Status code for the response
    std::vector<header> headers; // Headers included in the response
    std::string         content; // Body of the response
};


namespace misc_string {

// Strings used in forming an HTTP response
const char field_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

} // namespace helper_strings


namespace default_responses {

// Gets default message body for a given status code
std::string to_string(response::status_code status);

} // namespace default_responses


namespace status_string {

// Status lines for every status code in HTTP/1.0
const std::string ok =
    "HTTP/1.0 200 OK\r\n";
const std::string created =
    "HTTP/1.0 201 Created\r\n";
const std::string accepted =
    "HTTP/1.0 202 Accepted\r\n";
const std::string no_content =
    "HTTP/1.0 204 No Content\r\n";
const std::string moved_permanently =
    "HTTP/1.0 301 Moved Permanently\r\n";
const std::string moved_temporarily =
    "HTTP/1.0 302 Moved Temporarily\r\n";
const std::string not_modified =
    "HTTP/1.0 304 Not Modified\r\n";
const std::string bad_request =
    "HTTP/1.0 400 Bad Request\r\n";
const std::string unauthorized =
    "HTTP/1.0 401 Unauthorized\r\n";
const std::string forbidden =
    "HTTP/1.0 403 Forbidden\r\n";
const std::string not_found =
    "HTTP/1.0 404 Not Found\r\n";
const std::string internal_server_error =
    "HTTP/1.0 500 Internal Server Error\r\n";
const std::string not_implemented =
    "HTTP/1.0 501 Not Implemented\r\n";
const std::string bad_gateway =
    "HTTP/1.0 502 Bad Gateway\r\n";
const std::string service_unavailable =
    "HTTP/1.0 503 Service Unavailable\r\n";

} // namespace status_string

} // namespace http

#endif // HTTP_RESPONSE_H

