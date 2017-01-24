#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;


// Represents a session between the server and a client
class session : public std::enable_shared_from_this<session> {
public:

    // Constructor
    session(tcp::socket sock);

    // Starts the session between client and server
    void start();

private:

    // Max length of a single read from the client
    enum { max_length = 1024 };

    // Callback for when a client should have its data read
    void do_read();

    // Callback for when a client should be written to
    void do_write(std::size_t length);

    char        data[max_length]; // Buffer used when reading data from client
    tcp::socket socket;           // Used in boost.asio to represent a client
};


// Represents the server
class server  {
public:

    // Constructor
    server(boost::asio::io_service& io_service, short port);

private:

    // Callback for when a client attempts to connect
    void do_accept();

    tcp::acceptor acceptor; // Used in boost.asio to take in new clients
    tcp::socket   socket;   // Used in boost.asio to represent clients
};

#endif // SERVER_H

