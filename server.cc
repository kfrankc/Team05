//
// async_tcp_echo_server.cc
// based off of: http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/example/cpp11/echo/async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "server.h"

using boost::asio::ip::tcp;


session::session(tcp::socket socket)
  : socket_(std::move(socket)) {
}

void session::start() {
    do_read();
}

// read up to max_length bytes into data_ and calls do_write
void session::do_read() {

    

    auto self(shared_from_this());

    // will do async read, up to max_length bytes; stored in data  with length updated to reflect how much was read
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length) {
            // if no error (connection still open on other side)
            if (!ec) {
                printf("Read data:\n");
                for (std::size_t i = 0; i< length; i++) {
                    printf("%c", data_[i]);
                }
                printf("===========\n");

                do_write(length);
            }
            
        });
}

// called to write back out over the socket, modified
// will cycle between write and read until nothing is left to write
void session::do_write(std::size_t length) {

    

    auto self(shared_from_this());

    // demonstration that the data read in can be modified here before being written out
    for (unsigned int i = 0; i < length; i++) {
        if(isalpha(data_[i])) {
            data_[i] = data_[i] + 1; // increment the value to be the next ACII character
        }
    }

    // async write out socket
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
    [this, self](boost::system::error_code ec, std::size_t len) {
        if (!ec) {
            printf("Echoed (modified) data:\n");
            for (std::size_t i = 0; i< len; i++) {
                printf("%c", data_[i]);
            }
            printf("===========\n\n");

            do_read();
        }
    });
}




server::server(boost::asio::io_service& io_service, short port)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    socket_(io_service) {

        do_accept();
}

// called when server is started and repeatedly called
void server::do_accept()
{
    acceptor_.async_accept(socket_,
    [this](boost::system::error_code ec) {
        if (!ec) {
            // creates a shared pointer of session?
            // calls start function of session class
            std::make_shared<session>(std::move(socket_))->start();
        }

        // repeats this
        do_accept();
    });
}


