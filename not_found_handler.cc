#include "not_found_handler.h"

RequestHandler::Status NotFoundHandler::Init(const std::string& uri_prefix,
                                             const NginxConfig& config) 
    : request_uri_prefix(uri_prefix), request_config(config) 
{
    std::cout << "NotFoundHandler::Init called" << std::endl;
    return RequestHandler::OK;
}

RequestHandler::Status NotFoundHandler::HandleRequest(const Request& request,
                                                      Response* response) {
    std::cout << "NotFoundHandler::HandleRequest called" << std::endl;
    return RequestHandler::OK;
}

