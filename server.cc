#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "http_response.h"
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
    socket.async_read_some(boost::asio::buffer(data, max_length),
        [this, self](boost::system::error_code ec, std::size_t len) {
            // Check if an error has occurred during the connection
            if (!ec) {
                // No error : move on to echoing the data to the client
                do_write(len);
            }
        });
}


// Callback for when a client should be written to
void session::do_write(std::size_t length) {
    // Create a reference to "this" to ensure it outlives the async operation
    auto self(shared_from_this());

    // Setup an HTTP response from the data received from the client
    http::response r =
        http::response::plain_text_response(std::string(data, length));

    // Send the response back to the client and then we're done
    boost::asio::async_write(socket, r.to_buffers(),
        [this, self](boost::system::error_code ec, std::size_t len) {
            // Check if an error has occurred during the connection
            if (!ec) {
                // No error : print out the data we sent to the client just now
                printf("Echoed the following\n");
                printf("==========\n");
                for (std::size_t i = 0; i < len; i++) {
                    printf("%c", data[i]);
                }
                printf("\n==========\n\n\n");
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

