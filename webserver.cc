#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "async_tcp_echo_server.h"
#include "config_parser.h"

using boost::asio::ip::tcp;




int main(int argc, char* argv[]) {

    try {
        if (argc != 2) {
            std::cerr << "Usage: webserver <port>\n";
            return 1;
        }


        NginxConfigParser config_parser;
        NginxConfig config;
        config_parser.Parse("example_config", &config);

        printf("%s", config.ToString().c_str());

        boost::asio::io_service io_service;

        server s(io_service, std::atoi(argv[1]));


        printf("Running server on port %s...\n", argv[1]);


        io_service.run();




  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}