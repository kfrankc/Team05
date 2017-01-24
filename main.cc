#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "server.h"
#include "config_parser.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {

    try {
        if (argc != 2) {
            std::cerr << "Usage: server <config_file>" << std::endl;
            return 1;
        }


        NginxConfigParser config_parser;
        NginxConfig config;
        config_parser.Parse(argv[1], &config);
        int port = 0;

        for (size_t i = 0; i < config.statements_.size(); i++) {
            if (config.statements_[i]->tokens_[0] == "port") {
                port = std::stoi(config.statements_[i]->tokens_[1]);
            } 
        }

        printf("%s", config.ToString().c_str());

        boost::asio::io_service io_service;
        
        if (port == 0) {
            std::cerr << "Missing port <number>; in config file." << std::endl;
            return 1;
        }
        server s(io_service, port);


        printf("Running server on port %d...\n", port);


        io_service.run();




  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
