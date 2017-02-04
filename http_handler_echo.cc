#include "http_handler_echo.h"
#include "http_request.h"
#include "http_response.h"


namespace http {

// Echoes the request as plain text
response handler_echo::handle_request(const request& req) {
    return http::response::plain_text_response(std::string(req.string));
}

} // namespace http

