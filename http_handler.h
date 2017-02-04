#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP


namespace http {

struct response;
struct request;


// Abstract base class for serving all incoming HTTP requests
class handler {
public:

    // Returns a response to the given request
    virtual response handle_request(const request& req) = 0;
};

} // namespace http

#endif // HTTP_HANDLER_HPP

