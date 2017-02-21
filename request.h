// Represents an HTTP Request.
//
// Usage:
//   auto request = Request::Parse(raw_request);

#ifndef REQUEST_H
#define REQUEST_H

class Request {
public:
    // Parse raw request string read in from socket
    static std::unique_ptr<Request> Parse(const std::string& raw_request);
    
    // Get the raw request string
    std::string raw_request() const;
    // Get the method type string
    std::string method() const;
    // Get the URI string
    std::string uri() const;
    // Get the version string
    std::string version() const;
    
    // Get the headers of the request
    using Headers = std::vector<std::pair<std::string, std::string>>;
    Headers headers() const;
    // Get the body of the request
    std::string body() const;

private:
    std::string raw_request;  // Raw request
    std::string method;       // Method type
    std::string uri;          // URI
    std::string version;      // Version
    Headers headers;          // Headers of request
    std::string body;         // Body of request
};


#endif // REQUEST_H
