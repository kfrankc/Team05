#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "server.h"
#include "server_config_parser.h"
#include "request_handler.h"

using boost::asio::ip::tcp;


// Entry point of the program
int main(int argc, char* argv[]) {
    try {
        // Make sure a config file is passed to the program
        if (argc != 2) {
            std::cerr << "Usage: webserver <config_file>" << std::endl;
            return 1;
        }
	
        // Parse the config file into statements and tokens
        NginxServerConfigParser server_parser(argv[1]);

        // Get the server settings from the config file (i.e. port)
        ServerSettings server_settings;
        int port = server_parser.ParseServerSettings(server_settings);
        if (port == -1) {
            std::cerr << "Missing port <number> in config file" << std::endl;
            return 1;
        }

        // Parse the request handlers from the config file
        std::map<std::string, std::unique_ptr<RequestHandler> > handlers;
        std::map<std::string, std::string> handler_info;
        server_parser.ParseRequestHandlers(handlers, handler_info);
        printf("%lu handlers parsed\n", (unsigned long int)(handlers.size()));

        // Start the server
        printf("Running server on port %d...\n\n", port);
        Server::Run(port, std::move(handlers), std::move(handler_info));
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

