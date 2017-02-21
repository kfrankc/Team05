#include "request_handler.h"
#include "response.h"
#include "request.h"
#include "config_parser.h"
#include <string>

int main(int arc, char** argv) {

  NginxConfig config;
  std::string str;
  Request request;
  Response response;

  auto handler = RequestHandler::CreateByName("EchoHandler");
  handler->Init(str, config);
  handler->HandleRequest(request, &response);
  handler = RequestHandler::CreateByName("StaticFileHandler");
  handler->Init(str, config);
  handler->HandleRequest(request, &response);
  return 0;
}