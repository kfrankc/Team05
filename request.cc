#include "request.h"

std::unique_ptr<Request> Request::Parse(const std::string& raw_request) {
    std::unique_ptr<Request> req = std::unique_ptr<Request>(new Request);
    // TODO: Parse the raw request for method, uri, version, body, headers, etc
    return req;
}

std::string Request::raw_request() const {
    return raw_request;
}

std::string Request::method() const {
    return method;
}

std::string Request::uri() const {
    return uri;
}

std::string Request::version() const {
    return version;
}

Headers Request::headers() const {
    return headers;
}

std::string Request::body() const {
    return body;
}

