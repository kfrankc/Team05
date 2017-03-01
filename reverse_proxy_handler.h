#ifndef REVERSE_PROXY_HANDLER_H
#define REVERSE_PROXY_HANDLER_H

#include "request_handler.h"

// Retrieves a file from the server's file system
class ReverseProxyHandler : public RequestHandler {
public:

    // Initializes the handler. Returns OK if successful
    // uri_prefix is the value in the config file that this handler will run for
    // config is the contents of the child block for this handler ONLY
    virtual Status Init(const std::string& uri_prefix,
                        const NginxConfig& config);

    // Handles an HTTP request, and generates a response. Returns a response code
    // indicating success or failure condition. If ResponseCode is not OK, the
    // contents of the response object are undefined, and the server will return
    // HTTP code 500
    virtual Status HandleRequest(const Request& request, Response* response);

private:

    std::string original_uri_prefix;
    std::string remote_host;
    std::string remote_port;
};

REGISTER_REQUEST_HANDLER(ReverseProxyHandler);

#endif  // REVERSE_PROXY_HANDLER_H
