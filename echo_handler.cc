#include "request_handler.h"
#include "echo_handler.h"

RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix, 
                                         const NginxConfig& config) 
    : request_uri_prefix(uri_prefix), request_config(config)
{
    std::cout << "EchoHandler::Init called" << std::endl;
    return RequestHandler::OK;
}

RequestHandler::Status EchoHandler::HandleRequest(const Request& request, 
                                                  Response* response) {
    std::cout << "EchoHandler::HandleRequest called" << std::endl;
    return RequestHandler::OK;
}
