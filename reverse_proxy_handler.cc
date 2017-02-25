#include "reverse_proxy_handler.h"

// Initializes the handler. Returns OK if successful
// uri_prefix is the value in the config file that this handler will run for
// config is the contents of the child block for this handler ONLY
RequestHandler::Status ReverseProxyHandler::Init(const std::string& uri_prefix,
const NginxConfig& config) {
    // TODO
    return RequestHandler::OK;
}

// Handles an HTTP request, and generates a response. Returns a response code
// indicating success or failure condition. If ResponseCode is not OK, the
// contents of the response object are undefined, and the server will return
// HTTP code 500
RequestHandler::Status ReverseProxyHandler::HandleRequest(const Request& request,
Response* response) {
    // TODO
    return RequestHandler::OK;
}


