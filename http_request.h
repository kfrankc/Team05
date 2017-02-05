#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <vector>
#include "http_header.h"


namespace http {

// HTTP/1.0 request received from a client
struct request {
    std::string         method;  // Indicates what is to be performed
    std::string         uri;     // Identifies the resource for the request
    std::string         path;    // URL converted to a path
    std::vector<header> headers; // Headers included in the request
    std::string         string;  // The entire, unparsed request as a string
};

} // namespace http

#endif // HTTP_REQUEST_H

