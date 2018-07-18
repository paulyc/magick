//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdio>
#include <iostream>
#include <regex>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <signal.h>
#include "server.hpp"
#include "request_handler.hpp"
#include "reply.hpp"

int main(int argc, char* argv[])
{
    struct {
        std::string bind_addr;
        std::string port;
        std::string account_sid;
        std::string auth_token;
    } config;
    
  try
  {
      std::string exec_name(argv[0]);
      config.bind_addr = argc > 1 ? argv[1] : "0.0.0.0";
      config.port = argc > 2 ? argv[2] : "80";
      config.account_sid = "itsasecret";
      config.auth_token = "itsasecret";
      
    // Check command line arguments.
    if (argc > 3 || (argc > 1 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))))
    {
      std::cerr << "Usage: " << argv[0] << " [<address> [port]]\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    " << argv[0] << " 0.0.0.0 80 .\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    " << argv[0] << " 0::0 80 .\n";
      return 1;
    }

    boost::asio::io_context io_context;
      server4::request_handler handler;
      handler.route("POST", std::regex("/fax/sent"), [&config](server4::request& req, server4::reply& rep) {
          rep = server4::reply::stock_reply(server4::reply::ok);
      });
      handler.route("POST", std::regex("/fax/received"), [&config](server4::request& req, server4::reply& rep) {
          rep = server4::reply::stock_reply(server4::reply::ok);
      });
      handler.route("PUT", std::regex("/fax/send"), [&config](server4::request& req, server4::reply& rep) {
          rep = server4::reply::stock_reply(server4::reply::ok);
      });

    // Launch the initial server coroutine.
    server4::server(io_context, config.bind_addr, config.port, handler)();

    // Wait for signals indicating time to shut down.
    boost::asio::signal_set signals(io_context);
    signals.add(SIGINT);
    signals.add(SIGTERM);
#if defined(SIGQUIT)
    signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
    signals.async_wait(boost::bind(
          &boost::asio::io_context::stop, &io_context));

    // Run the server.
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}
