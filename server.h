#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "request.h"
#include "request_handler.h"
#include "response.h"
#include "echo_handler.h"
#include "static_file_handler.h"

using boost::asio::ip::tcp;


// Represents a session between the server and a client
class session : public std::enable_shared_from_this<session> {
public:

    // Constructor taking a list of HTTP request handlers
    session(tcp::socket sock,
        const std::map<std::string, std::unique_ptr<RequestHandler> >& hndlers);

    // Starts the session between client and server
    void start();

private:

    // Max length of a single read from the client
    enum { max_length = 8192 };

    // Callback for when a client should have its data read
    void do_read();

    // Callback for when a client should be written to
    void do_write(const Response& res);

    // Reference to the vector of request handlers
    const std::map<std::string, std::unique_ptr<RequestHandler> >& handlers;

    std::array<char, max_length> buf; // Buffer used when reading client data
    // http::request_parser parser;      // Parser for incoming client requests
    Request        request;     // Structure for storing client requests
    tcp::socket          socket;      // Used to represent a client
};


// Represents the server
class server  {
public:

    // Constructor taking a list of HTTP request handlers
    server(boost::asio::io_service& io_service, int port,
        const std::map<std::string, std::unique_ptr<RequestHandler> >& hndlers);

private:

    // Callback for when a client attempts to connect
    void do_accept();

    // Vector of request handlers - used across multiple sessions/threads
    const std::map<std::string, std::unique_ptr<RequestHandler> >& handlers;

    tcp::acceptor acceptor; // Used in boost.asio to take in new clients
    tcp::socket   socket;   // Used in boost.asio to represent clients
};

#endif // SERVER_H

