#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "server.h"
#include "config_parser.h"

using boost::asio::ip::tcp;


// Entry point of the program
int main(int argc, char* argv[]) {
    try {
        // Make sure a config file is passed to the program
        if (argc != 2) {
            std::cerr << "Usage: server <config_file>" << std::endl;
            return 1;
        }

        // Parses the config file
        NginxConfigParser config_parser;
        NginxConfig config;
        config_parser.Parse(argv[1], &config);
        
        // Extracts the port number from the config file
        int port = 0;
        for (size_t i = 0; i < config.statements.size(); i++) {
            if (config.statements[i]->tokens[0] == "port") {
                port = std::stoi(config.statements[i]->tokens[1]);
            } 
        }
        if (port == 0) {
            std::cerr << "Missing port <number> in config file" << std::endl;
            return 1;
        }

        // Start the server
        boost::asio::io_service io_service;
        server s(io_service, port);
        printf("Running server on port %d...\n", port);
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

