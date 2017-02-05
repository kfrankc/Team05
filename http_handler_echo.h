#ifndef HTTP_HANDLER_ECHO_HPP
#define HTTP_HANDLER_ECHO_HPP

#include "http_handler.h"


namespace http {

struct response;
struct request;


// Handler for serving HTTP echo requests
class handler_echo : public handler {
public:

    // Echoes the request as plain text
    response handle_request(const request& req);
};

} // namespace http

#endif // HTTP_HANDLER_ECHO_HPP
