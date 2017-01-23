//
// async_tcp_echo_server.cc
// original from: http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/example/cpp11/echo/async_tcp_echo_server.cpp
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
#include "async_tcp_echo_server.h"

using boost::asio::ip::tcp;


session::session(tcp::socket socket)
  : socket_(std::move(socket)) {
}

void session::start() {
    do_read();
}


void session::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                do_write(length);
            }
        });
}

void session::do_write(std::size_t length) {
    auto self(shared_from_this());
    for (unsigned int i = 0; i < length; i++) {
        if(isalpha(data_[i])) {
            data_[i] = data_[i] + 1; //increment the value to be the next ACII character
        }
    }
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
    [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
            do_read();
        }
    });
}




server::server(boost::asio::io_service& io_service, short port)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    socket_(io_service) {

        do_accept();
}


void server::do_accept()
{
    acceptor_.async_accept(socket_,
    [this](boost::system::error_code ec) {
        if (!ec) {
            std::make_shared<session>(std::move(socket_))->start();
        }

        do_accept();
    });
}


