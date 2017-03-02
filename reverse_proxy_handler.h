#ifndef REVERSE_PROXY_HANDLER_H
#define REVERSE_PROXY_HANDLER_H

#include "request_handler.h"

// Interacts with clients from the Internet at-large
// as if this server was the one serving up the
// content requested, rather than the remote servers
// to which it actually reaches out.
//
// See: https://github.com/UCLA-CS130/Mr.-Robot-et-al./issues/62
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

    // Helper for sending proxy requests
    // Returns the string representation of the response received from the remote_host
    // Origin is the remote_host that is the original source of the content
    // we're serving on their behalf
    std::string sendRequestToOrigin(const std::string& remote_uri);
};

REGISTER_REQUEST_HANDLER(ReverseProxyHandler);

#endif  // REVERSE_PROXY_HANDLER_H
