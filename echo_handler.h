#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include "request_handler.h"
#include <iostream>

class EchoHandler : public RequestHandler {
public:
    Status Init(const std::string& uri_prefix, const NginxConfig& config);

    Status HandleRequest(const Request& request, Response* response);
};

REGISTER_REQUEST_HANDLER(EchoHandler);

#endif  // ECHO_HANDLER_H
