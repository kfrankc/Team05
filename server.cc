// This file is based on both the Boost HTTP server and Echo example at
//  http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/examples/cpp11_examples.html

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "request.h"
#include "response.h"
#include "server.h"

using boost::asio::ip::tcp;


// Constructor taking a list of HTTP request handlers
session::session(tcp::socket sock,
const std::map<std::string, std::unique_ptr<RequestHandler> >& hndlers) :
handlers(hndlers), socket(std::move(sock)) {

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
                Request request;
                Request::Result result;
                std::tie(result, std::ignore) = request.Parse(buf.data(),
                    buf.data() + len);
                if (result == Request::Result::good) {
                    // Get the path prefix

                    std::string path;
                    if(!request.path(path)) {
                        // TODO: figure out status code
                    }
                    std::string path_prefix = path.substr(0,
                        path.find("/", 1)); 

                    // Handle the request or return response code 500 if not OK
                    Response response;
                    std::map<std::string, std::unique_ptr<RequestHandler> >::const_iterator it;
                    it = handlers.find(path_prefix);
                    RequestHandler::Status status;
                    if (it != handlers.end()) {
                        status = it->second->HandleRequest(request, &response);
                    } else {
                        // TODO: Error handle
                    }
                    if (status == RequestHandler::OK) {
                        do_write(response);
                    } else {
                        do_write(Response::DefaultResponse(Response::internal_server_error));
                    }
                } else if (result == Request::bad) {
                    do_write(Response::DefaultResponse(Response::bad_request));
                } else {
                    do_read();
                }
            }
        });
}


// Callback for when a client should be written to
void session::do_write(const Response& res) {
    // Create a reference to "this" to ensure it outlives the async operation
    auto self(shared_from_this());

    // // Print out the data we're sending to the client
    // printf("Sending the following to %s\n",
    //     socket.remote_endpoint().address().to_string().c_str());
    // printf("==========\n");
    // printf("%s\n", request.as_string.c_str());
    // printf("==========\n\n");

    // Send the response back to the client and then we're done
    std::string res_str = res.ToString();
    boost::asio::async_write(socket, boost::asio::buffer(res_str, res_str.size()),
        [this, self](boost::system::error_code ec, std::size_t len) {
            if (ec) {
                printf("Failed to send data to %s\n\n",
                    socket.remote_endpoint().address().to_string().c_str());
            }
            printf("Amount of data written: %zu\n\n", len);
        });
}


// Constructor taking a list of HTTP request handlers
server::server(boost::asio::io_service& io_service, int port,
const std::map<std::string, std::unique_ptr<RequestHandler> >& hndlers) :
handlers(hndlers), acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
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
                std::make_shared<session>(std::move(socket), handlers)->start();
            }

            // Repeatedly do this
            do_accept();
        });
}

