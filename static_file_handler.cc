#include "static_file_handler.h"


RequestHandler::Status StaticFileHandler::Init(const std::string& uri_prefix, 
                                               const NginxConfig& config) 
    : request_uri_prefix(uri_prefix), request_config(config)
{
    std::cout << "StaticFileHandler::Init called" << std::endl;
    return RequestHandler::OK;
}

RequestHandler::Status StaticFileHandler::HandleRequest(const Request& request,
                                                        Response* response) {
    std::cout << "StaticFileHandler::HandleRequest called" << std::endl;
    return RequestHandler::OK;
}
