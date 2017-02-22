#include "server_config_parser.h"
#include "request_handler.h"


// Constructor: pass in a config file to be parsed for its contents
NginxServerConfigParser::NginxServerConfigParser(const char* config_file) {
    // Parse the config file into statements and tokens for us to extract data
    config_parser.Parse(config_file, &config);
}


// Parses all server request handlers and stores the parsed
// handlers in a vector out-param. Returns number of request handlers found
int NginxServerConfigParser::ParseRequestHandlers(
std::map<std::string, std::unique_ptr<RequestHandler> >& handlers_out) {
    // Check config file for request handlers to add
    for (size_t i = 0; i < config.statements.size(); i++) {
        // TODO: Add validation to config file syntax
        if (config.statements[i]->tokens[0] == "path") {
            auto handler = RequestHandler::CreateByName(
                              config.statements[i]->tokens[2].c_str());
            handler->Init(config.statements[i]->tokens[1],
                         *(config.statements[i]->child_block));
            handlers_out.insert(std::pair<std::string, std::unique_ptr<RequestHandler> >(
                          config.statements[i]->tokens[1],
                          std::move(std::unique_ptr<RequestHandler>(handler))));
        }
    }

    return handlers_out.size();
}


// Parses all server settings and stores the parsed server setup in 
// server_config out-parm. Returns port number (-1 if no number is found)
int NginxServerConfigParser::ParseServerSettings(
ServerSettings& server_settings_out) {
    server_settings_out.port = -1;

    for (size_t i = 0; i < config.statements.size(); i++) {
        if (config.statements[i]->tokens[0] == "port") {
            server_settings_out.port =
                                  std::stoi(config.statements[i]->tokens[1]);
        }
    }

    return server_settings_out.port;
}
