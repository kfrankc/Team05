#include "status_handler.h"

RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix,
                                           const NginxConfig& config) 
    : request_uri_prefix(uri_prefix), request_config(config)
{
    std::cout << "StatusHandler::Init called" << std::endl;
    return RequestHandler::OK;
}

RequestHandler::Status StatusHandler::HandleRequest(const Request& request, 
                                                    Response* response) {
    std::cout << "StatusHandler::HandleRequest called" << std::endl;
    return RequestHandler::OK;
}
 
