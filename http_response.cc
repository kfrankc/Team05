#include "http_response.h"


namespace http {

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
std::string to_string(response::status_code status)
{
    switch (status)
    {
    case response::ok:
        return ok;
    case response::created:
        return created;
    case response::accepted:
        return accepted;
    case response::no_content:
        return no_content;
    case response::moved_permanently:
        return moved_permanently;
    case response::moved_temporarily:
        return moved_temporarily;
    case response::not_modified:
        return not_modified;
    case response::bad_request:
        return bad_request;
    case response::unauthorized:
        return unauthorized;
    case response::forbidden:
        return forbidden;
    case response::not_found:
        return not_found;
    case response::internal_server_error:
        return internal_server_error;
    case response::not_implemented:
        return not_implemented;
    case response::bad_gateway:
        return bad_gateway;
    case response::service_unavailable:
        return service_unavailable;
    default:
        return internal_server_error;
    }
}

} // namespace default_responses




namespace status_string {


// Gets status line for a given status code
boost::asio::const_buffer to_buffer(response::status_code status) {
    switch (status) {
    case response::ok:
        return boost::asio::buffer(ok);
    case response::created:
        return boost::asio::buffer(created);
    case response::accepted:
        return boost::asio::buffer(accepted);
    case response::no_content:
        return boost::asio::buffer(no_content);
    case response::moved_permanently:
        return boost::asio::buffer(moved_permanently);
    case response::moved_temporarily:
        return boost::asio::buffer(moved_temporarily);
    case response::not_modified:
        return boost::asio::buffer(not_modified);
    case response::bad_request:
        return boost::asio::buffer(bad_request);
    case response::unauthorized:
        return boost::asio::buffer(unauthorized);
    case response::forbidden:
        return boost::asio::buffer(forbidden);
    case response::not_found:
        return boost::asio::buffer(not_found);
    case response::internal_server_error:
        return boost::asio::buffer(internal_server_error);
    case response::not_implemented:
        return boost::asio::buffer(not_implemented);
    case response::bad_gateway:
        return boost::asio::buffer(bad_gateway);
    case response::service_unavailable:
        return boost::asio::buffer(service_unavailable);
    default:
        return boost::asio::buffer(internal_server_error);
    }
}

} // namespace status_string


// Creates a default response for a given status code
response response::default_response(response::status_code status)
{
    response r;
    r.status = status;
    r.content = default_responses::to_string(status);
    r.headers.resize(2);
    r.headers[0].name = "Content-Length";
    r.headers[0].value = std::to_string(r.content.size());
    r.headers[1].name = "Content-Type";
    r.headers[1].value = "text/html";
    return r;
}


// Creates a text/html response for the given text or html
response response::text_or_html_response(std::string&& text_or_html)
{
    response r;
    r.status = response::ok;
    r.content = std::move(text_or_html);
    r.headers.resize(2);
    r.headers[0].name = "Content-Length";
    r.headers[0].value = std::to_string(r.content.size());
    r.headers[1].name = "Content-Type";
    r.headers[1].value = "text/plain";
    return r;
}

response response::plain_text_response(std::string&& text_or_html)
{
    response r;
    r.status = response::ok;
    r.content = std::move(text_or_html);
    r.headers.resize(2);
    r.headers[0].name = "Content-Length";
    r.headers[0].value = std::to_string(r.content.size());
    r.headers[1].name = "Content-Type";
    r.headers[1].value = "text/plain";
    return r;
}

response response::html_text_response(std::string&& text_or_html)
{
    response r;
    r.status = response::ok;
    r.content = std::move(text_or_html);
    r.headers.resize(2);
    r.headers[0].name = "Content-Length";
    r.headers[0].value = std::to_string(r.content.size());
    r.headers[1].name = "Content-Type";
    r.headers[1].value = "text/html";
    return r;
}





// Converts the response into buffers so that it can be sent to the client
std::vector<boost::asio::const_buffer> response::to_buffers() {
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(status_string::to_buffer(status));
    for (std::size_t i = 0; i < headers.size(); ++i) {
        const header& h = headers[i];
        buffers.push_back(boost::asio::buffer(h.name));
        buffers.push_back(boost::asio::buffer(misc_string::field_separator));
        buffers.push_back(boost::asio::buffer(h.value));
        buffers.push_back(boost::asio::buffer(misc_string::crlf));
    }
    buffers.push_back(boost::asio::buffer(misc_string::crlf));
    buffers.push_back(boost::asio::buffer(content));
    return buffers;
}

} // namespace http

