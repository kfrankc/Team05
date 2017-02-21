#ifndef STATUS_HANDLER_H
#define STATUS_HANDLER_H

#include "request_handler.h"
#include <iostream>

class StatusHandler : public RequestHandler {
public:
    // Initialize status handler with URI prefix and config child block
    Status Init(const std::string& uri_prefix, const NginxConfig& config);
    
    // Displays status of web server such as how many requests received
    // and what request handler/URL prefix pairs exist
    Status HandleRequest(const Request& request, Response* response);
};

REGISTER_REQUEST_HANDLER(StatusHandler);

#endif // STATUS_HANDER_H

