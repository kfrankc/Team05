#include "static_file_handler.h"


RequestHandler::Status StaticFileHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
  std::cout << "StaticFileHandler::Init called" << std::endl;
}

RequestHandler::Status StaticFileHandler::HandleRequest(const Request& request, Response* response) {
  std::cout << "StaticFileHandler::HandleRequest called" << std::endl;
}