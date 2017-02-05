// This file is based on both the Boost HTTP server and Echo example at
//  http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/examples/cpp11_examples.html


#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "http_handler_echo.h"
#include "http_handler_file.h"
#include "server.h"

using boost::asio::ip::tcp;


// Constructor
session::session(tcp::socket sock) :
    socket(std::move(sock)) {

}


// Starts the session between client and server
void session::start() {
    do_read();
}


// Callback for when a client should have its data read
void session::do_read() {
    // Create a reference to "this" to ensure it outlives the async operation
    auto self(shared_from_this());

    // Read up to max_length bytes from the client then write it back
    socket.async_read_some(boost::asio::buffer(buf),
        [this, self](boost::system::error_code ec, std::size_t len) {
            // Check if an error has occurred during the connection
            if (!ec) {
                // No error : parse the data from the client
                http::request_parser::result rslt;
                std::tie(rslt, std::ignore) = parser.parse(
                    request, buf.data(), buf.data() + len);
                if (rslt == http::request_parser::good) {
                    // TODO: Use the config file to create'n'choose handlers
                    if (request.path == "/echo") {
                        http::handler_echo handler;
                        do_write(handler.handle_request(request));
                    } else {
                        http::handler_file handler("./");
                        do_write(handler.handle_request(request));
                    }
                } else if (rslt == http::request_parser::bad) {
                    do_write(http::response::default_response(
                        http::response::bad_request));
                } else {
                    do_read();
                }
            }
        });
}


// Callback for when a client should be written to
void session::do_write(const http::response& res) {
    // Create a reference to "this" to ensure it outlives the async operation
    auto self(shared_from_this());

    // Print out the data we're sending to the client
    printf("Sending the following to %s\n",
        socket.remote_endpoint().address().to_string().c_str());
    printf("==========\n");
    printf("%s\n", request.as_string.c_str());
    printf("==========\n\n");

    // Send the response back to the client and then we're done
    boost::asio::async_write(socket, res.to_buffers(),
        [this, self](boost::system::error_code ec, std::size_t len) {
            if (ec) {
                printf("Failed to send data to %s\n\n",
                    socket.remote_endpoint().address().to_string().c_str());
            }
        });
}


// Constructor
server::server(boost::asio::io_service& io_service, short port) :
    acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
    socket(io_service) {
    // Start accepting clients as soon as the server instance is created
    do_accept();
}


// Callback for when a client attempts to connect
void server::do_accept()
{
    // Accept clients continuously in other threads than the main thread
    acceptor.async_accept(socket,
        [this](boost::system::error_code ec) {
            // Check if an error has occurred during the connection
            if (!ec) {
                // No error : creates a session between the client and server
                std::make_shared<session>(std::move(socket))->start();
            }

            // Repeatedly do this
            do_accept();
        });
}

