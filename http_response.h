#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

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

    // Creates a text/html response for the given text or html
    static response text_or_html_response(std::string&& text_or_html);

    // Converts the response into buffers so that it can be sent to the client
    std::vector<boost::asio::const_buffer> to_buffers();

    status_code         status;  // Status code for the response
    std::vector<header> headers; // Headers included in the response
    std::string         content; // Body of the response
};

} // namespace http

#endif // HTTP_RESPONSE_HPP

