//
// file_handler.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER4_FILE_HANDLER_HPP
#define HTTP_SERVER4_FILE_HANDLER_HPP

#include <string>
#include <regex>
#include <map>
#include <functional>

namespace server4 {

struct reply;
struct request;

/// The common handler for all incoming requests.
class request_handler
{
public:
  request_handler();
    virtual ~request_handler() {}

  /// Handle a request and produce a reply.
  void operator()(const request& req, reply& rep);
    
    void route(const std::string &method, const std::regex &regex, const std::function<void(request&, reply&)> &fun);
    void route(const std::string &method, const std::regex &regex, const std::function<void(request&, reply&)> &fun, int priority);

private:
  /// The directory containing the files to be served.
  //std::string doc_root_;

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool url_decode(const std::string& in, std::string& out);
    
    std::map<std::string, std::map<int, std::pair<std::regex, std::function<void(request&, reply&)>>>> route_map;
};

} // namespace server4

#endif // HTTP_SERVER4_FILE_HANDLER_HPP
