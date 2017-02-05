#include "server_config_parser.h"


// Returns the port number from config_file or -1 if no number is found
//   TODO: Put this in a class as a static method
int port_number(const char* config_file) {
    int port = -1;
    NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse(config_file, &config);
    for (size_t i = 0; i < config.statements.size(); i++) {
        if (config.statements[i]->tokens[0] == "port") {
            port = std::stoi(config.statements[i]->tokens[1]);
        } 
    }
    return port;
}

