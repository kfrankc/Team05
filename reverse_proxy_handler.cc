#include "reverse_proxy_handler.h"

// Initializes the handler. Returns OK if successful
// uri_prefix is the value in the config file that this handler will run for
// config is the contents of the child block for this handler ONLY
RequestHandler::Status ReverseProxyHandler::Init(const std::string& uri_prefix,
const NginxConfig& config) {
    // Check config child block for settings
    for (size_t i = 0; i < config.statements.size(); i++) {
        // Only attempt to read statements with a first token we care about
        if (config.statements[i]->tokens.size() > 0) {
            if (config.statements[i]->tokens[0] == "remote_host") {
              // remote_host must have 2 tokens
              if (config.statements[i]->tokens.size() > 1) {
                  remote_host = config.statements[i]->tokens[1];
              } else {
                  printf("\"remote_host\" statement in config needs more tokens\n");
              }
            }
            if (config.statements[i]->tokens[0] == "remote_port") {
              // remote_port must have 2 tokens
              if (config.statements[i]->tokens.size() > 1) {
                  remote_port = config.statements[i]->tokens[1];
              } else {
                  printf("\"remote_port\" statement in config needs more tokens\n");
              }
            }
        }
    }
    if (!remote_host.empty() && !remote_port.empty()) {
        return RequestHandler::OK;
    } else {
        printf("Missing remote_port or remote_host provided to StaticFileHandler\n");
        return RequestHandler::Error;
    }
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


