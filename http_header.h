#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H

#include <string>


namespace http {

// Key-value pairs that are a part of an HTTP message
struct header {
    std::string name;  // Name of the field
    std::string value; // Value of the field
};

} // namespace http

#endif // HTTP_HEADER_H

