#include "echo_handler.h"


RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
  std::cout << "EchoHandler::Init called" << std::endl;
}

RequestHandler::Status EchoHandler::HandleRequest(const Request& request, Response* response) {
  std::cout << "EchoHandler::HandleRequest called" << std::endl;
}