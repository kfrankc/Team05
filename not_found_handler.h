#ifndef NOT_FOUND_HANDLER_H
#define NOT_FOUND_HANDLER_H

#include "request_handler.h"
#include <iostream>

class NotFoundHandler : public RequestHandler {
public:
    // Initialize 404 Not Found handler with URI prefix and config child block
    Status Init(const std::string& uri_prefix, const NginxConfig& config);

    // Requested 404 not found when static file handler cannot serve a file 
    Status HandleRequest(const Request& request, Response* response);
};

REGISTER_REQUEST_HANDLER(NotFoundHandler);

#endif // NOT_FOUND_HANDLER_H

