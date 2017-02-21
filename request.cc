#include "request.h"
#include <sstream>


// TODO Parses a request string into an instance of this class
std::unique_ptr<Request> Request::Parse(const std::string& raw_request) {
    std::unique_ptr<Request> req = std::unique_ptr<Request>(new Request);
    return req;
}


// Gets the unparsed string that represents the request
std::string Request::raw_request() const {
    return raw_request;
}


// Gets the method for the request (indicates what is to be performed)
std::string Request::method() const {
    return method;
}


// Gets the resource identifier for the request
std::string Request::uri() const {
    return uri;
}


// Returns true if out contains the URI converted to a file system path
bool Request::path(std::string& out) const {
    out.clear();
    out.reserve(uri.size());
    for (std::size_t i = 0; i < uri.size(); ++i) {
        if (uri[i] == '%') {
            if (i + 3 <= uri.size()) {
                int value = 0;
                std::istringstream is(uri.substr(i + 1, 2));
                if (is >> std::hex >> value) {
                    out += static_cast<char>(value);
                    i += 2;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else if (uri[i] == '+') {
            out += ' ';
        } else {
            out += uri[i];
        }
    }
    return true;
}


// Gets the HTTP version the request was made with
std::string Request::version() const {
    return version;
}


// Gets the headers of the request
Headers Request::headers() const {
    return headers;
}


// Gets the body of the request
std::string Request::body() const {
    return body;
}

