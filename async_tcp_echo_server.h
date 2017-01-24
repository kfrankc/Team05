// based off of: http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/example/cpp11/echo/async_tcp_echo_server.cpp


#ifndef ASYNC_TCP_ECHO_SERVER_H

#define ASYNC_TCP_ECHO_SERVER_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "async_tcp_echo_server.h"
using boost::asio::ip::tcp;


class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket);

  void start();

private:
  void do_read();

  void do_write(std::size_t length);

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
};

class server 
{
public:
  server(boost::asio::io_service& io_service, short port);

private:
  void do_accept();

  tcp::acceptor acceptor_;
  tcp::socket socket_;
};




#endif