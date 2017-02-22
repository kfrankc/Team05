#ifndef HTTP_HANDLER_ECHO_HPP
#define HTTP_HANDLER_ECHO_HPP

#include "http_handler.h"
#include "response.h"

namespace http {


struct request;


// Handler for serving HTTP echo requests
class handler_echo : public handler {
public:

    handler_echo(const std::string& base_url) : handler(base_url) {}; 

    // Echoes the request as plain text
    Response handle_request(const request& req);
};

} // namespace http

#endif // HTTP_HANDLER_ECHO_HPP

