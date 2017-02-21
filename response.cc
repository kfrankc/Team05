#include "response.h"




void Response::SetStatus(const ResponseCode Response_code) {
    this->status = Response_code;
}

void Response::AddHeader(const std::string& header_name, const std::string& header_value) {
    this->headers.push_back(std::make_pair(header_name, header_value));
}

void Response::SetBody(const std::string& body) {
    this->content = body;
}

Response::Headers Response::GetHeaders() const {
    return this->headers;
}

Response::ResponseCode Response::GetStatus() const {
    return this->status;
}

std::string Response::GetBody() const {
    return this->content;
}


namespace misc_string {

// Strings used in forming an HTTP Response
const char field_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

} // namespace helper_strings


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



namespace default_responses{

// Default message bodies for every status code in HTTP/1.0
const char ok[] =
    "";
const char created[] =
    "<html>"
    "<head><title>Created</title></head>"
    "<body><h1>201 Created</h1></body>"
    "</html>";
const char accepted[] =
    "<html>"
    "<head><title>Accepted</title></head>"
    "<body><h1>202 Accepted</h1></body>"
    "</html>";
const char no_content[] =
    "<html>"
    "<head><title>No Content</title></head>"
    "<body><h1>204 Content</h1></body>"
    "</html>";
const char moved_permanently[] =
    "<html>"
    "<head><title>Moved Permanently</title></head>"
    "<body><h1>301 Moved Permanently</h1></body>"
    "</html>";
const char moved_temporarily[] =
    "<html>"
    "<head><title>Moved Temporarily</title></head>"
    "<body><h1>302 Moved Temporarily</h1></body>"
    "</html>";
const char not_modified[] =
    "<html>"
    "<head><title>Not Modified</title></head>"
    "<body><h1>304 Not Modified</h1></body>"
    "</html>";
const char bad_request[] =
    "<html>"
    "<head><title>Bad Request</title></head>"
    "<body><h1>400 Bad Request</h1></body>"
    "</html>";
const char unauthorized[] =
    "<html>"
    "<head><title>Unauthorized</title></head>"
    "<body><h1>401 Unauthorized</h1></body>"
    "</html>";
const char forbidden[] =
    "<html>"
    "<head><title>Forbidden</title></head>"
    "<body><h1>403 Forbidden</h1></body>"
    "</html>";
const char not_found[] =
    "<html>"
    "<head><title>Not Found</title></head>"
    "<body><h1>404 Not Found</h1></body>"
    "</html>";
const char internal_server_error[] =
    "<html>"
    "<head><title>Internal Server Error</title></head>"
    "<body><h1>500 Internal Server Error</h1></body>"
    "</html>";
const char not_implemented[] =
    "<html>"
    "<head><title>Not Implemented</title></head>"
    "<body><h1>501 Not Implemented</h1></body>"
    "</html>";
const char bad_gateway[] =
    "<html>"
    "<head><title>Bad Gateway</title></head>"
    "<body><h1>502 Bad Gateway</h1></body>"
    "</html>";
const char service_unavailable[] =
    "<html>"
    "<head><title>Service Unavailable</title></head>"
    "<body><h1>503 Service Unavailable</h1></body>"
    "</html>";

// Gets default message body for a given status code
std::string to_string(Response::ResponseCode status)
{
    switch (status)
    {
    case Response::ok:
        return ok;
    case Response::created:
        return created;
    case Response::accepted:
        return accepted;
    case Response::no_content:
        return no_content;
    case Response::moved_permanently:
        return moved_permanently;
    case Response::moved_temporarily:
        return moved_temporarily;
    case Response::not_modified:
        return not_modified;
    case Response::bad_request:
        return bad_request;
    case Response::unauthorized:
        return unauthorized;
    case Response::forbidden:
        return forbidden;
    case Response::not_found:
        return not_found;
    case Response::internal_server_error:
        return internal_server_error;
    case Response::not_implemented:
        return not_implemented;
    case Response::bad_gateway:
        return bad_gateway;
    case Response::service_unavailable:
        return service_unavailable;
    default:
        return internal_server_error;
    }
}

} // namespace default_Responses


namespace status_string {

// Gets status line for a given status code
boost::asio::const_buffer to_buffer(Response::ResponseCode status) {
    switch (status) {
    case Response::ok:
        return boost::asio::buffer(ok);
    case Response::created:
        return boost::asio::buffer(created);
    case Response::accepted:
        return boost::asio::buffer(accepted);
    case Response::no_content:
        return boost::asio::buffer(no_content);
    case Response::moved_permanently:
        return boost::asio::buffer(moved_permanently);
    case Response::moved_temporarily:
        return boost::asio::buffer(moved_temporarily);
    case Response::not_modified:
        return boost::asio::buffer(not_modified);
    case Response::bad_request:
        return boost::asio::buffer(bad_request);
    case Response::unauthorized:
        return boost::asio::buffer(unauthorized);
    case Response::forbidden:
        return boost::asio::buffer(forbidden);
    case Response::not_found:
        return boost::asio::buffer(not_found);
    case Response::internal_server_error:
        return boost::asio::buffer(internal_server_error);
    case Response::not_implemented:
        return boost::asio::buffer(not_implemented);
    case Response::bad_gateway:
        return boost::asio::buffer(bad_gateway);
    case Response::service_unavailable:
        return boost::asio::buffer(service_unavailable);
    default:
        return boost::asio::buffer(internal_server_error);
    }
}


// Gets status line for a given status code
std::string to_string(Response::ResponseCode status) {
    switch (status) {
    case Response::ok:
        return (ok);
    case Response::created:
        return (created);
    case Response::accepted:
        return (accepted);
    case Response::no_content:
        return (no_content);
    case Response::moved_permanently:
        return (moved_permanently);
    case Response::moved_temporarily:
        return (moved_temporarily);
    case Response::not_modified:
        return (not_modified);
    case Response::bad_request:
        return (bad_request);
    case Response::unauthorized:
        return (unauthorized);
    case Response::forbidden:
        return (forbidden);
    case Response::not_found:
        return (not_found);
    case Response::internal_server_error:
        return (internal_server_error);
    case Response::not_implemented:
        return (not_implemented);
    case Response::bad_gateway:
        return (bad_gateway);
    case Response::service_unavailable:
        return (service_unavailable);
    default:
        return (internal_server_error);
    }
}


} // namespace status_string


// Creates a default Response for a given status code
Response Response::default_response(Response::ResponseCode status) {
    Response r;
    r.status = status;
    r.content = default_responses::to_string(status);
    r.headers.resize(2);
    r.headers[0].name = "Content-Length";
    r.headers[0].value = std::to_string(r.content.size());
    r.headers[1].name = "Content-Type";
    r.headers[1].value = "text/html";
    return r;
}

// Creates a text/plain Response for the given text or html
Response Response::plain_text_response(std::string&& text) {
    Response r;
    r.status = Response::ok;
    r.content = std::move(text);
    r.headers.resize(2);
    r.headers[0].name = "Content-Length";
    r.headers[0].value = std::to_string(r.content.size());
    r.headers[1].name = "Content-Type";
    r.headers[1].value = "text/plain";
    return r;
}

// Creates a text/html Response for the given text or html
Response Response::html_response(std::string&& html) {
    Response r;
    r.status = Response::ok;
    r.content = std::move(html);
    r.headers.resize(2);
    r.headers[0].name = "Content-Length";
    r.headers[0].value = std::to_string(r.content.size());
    r.headers[1].name = "Content-Type";
    r.headers[1].value = "text/html";
    return r;
}

std::string Response::ToString() const {
    std::string response_string = "";
    response_string += status_string::to_string(status);
    for (std::size_t i = 0; i < headers.size(); ++i) {
        const header& h = headers[i];
        response_string += (h.name);
        response_string += std::string(misc_string::field_separator, sizeof(misc_string::field_separator));
        response_string += (h.value);
        response_string += std::string(misc_string::crlf, sizeof(misc_string::crlf));
    }

    response_string += (misc_string::crlf);
    response_string += (content);
    return response_string;
}

