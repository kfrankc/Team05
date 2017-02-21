#include "http_handler_echo.h"
#include "http_request.h"



namespace http {

// Echoes the request as plain text
Response handler_echo::handle_request(const request& req) {
    // If base url is  not formatted correctly
    if (req.path.length() == 0 || req.path[0] != '/') {
        return Response::default_response(Response::internal_server_error);
    }

    return Response::plain_text_response(std::string(req.as_string));
}

} // namespace http

