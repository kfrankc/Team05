#include "status_handler.h"


// Initializes the handler. Returns OK if successful
// uri_prefix is the value in the config file that this handler will run for
// config is the contents of the child block for this handler ONLY
RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix, 
const NginxConfig& config) :
request_uri_prefix(uri_prefix),
request_config(config) {
    // TODO
    return RequestHandler::OK;
}


// Handles an HTTP request, and generates a response. Returns a response code
// indicating success or failure condition. If ResponseCode is not OK, the
// contents of the response object are undefined, and the server will return
// HTTP code 500
RequestHandler::Status StatusHandler::HandleRequest(const Request& request, 
Response* response) {
    // TODO
    *response = Response::DefaultResponse(Response::not_implemented);
    return RequestHandler::OK;
}

