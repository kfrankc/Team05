#ifndef REQUEST_H
#define REQUEST_H


// Represents an HTTP Request
//
// Usage:
//   auto request = Request::Parse(raw_request);
class Request {
public:

    // Parses a request string into an instance of this class
    static std::unique_ptr<Request> Parse(const std::string& raw_request);

    // Gets the unparsed string that represents the request
    std::string raw_request() const;

    // Gets the method for the request (indicates what is to be performed)
    std::string method() const;

    // Gets the resource identifier for the request
    std::string uri() const;

    // Returns true if out contains the URI converted to a file system path
    bool path(std::string& out) const;

    // Gets the HTTP version the request was made with
    std::string version() const;

    // Gets the headers of the request
    using Headers = std::vector<std::pair<std::string, std::string> >;
    Headers headers() const;

    // Gets the body of the request
    std::string body() const;

private:

    std::string raw;     // The entire, unparsed request as a string
    std::string method;  // Indicates what is to be performed
    std::string uri;     // Identifies the resource for the request
    std::string version; // HTTP version the requester is using
    Headers     headers; // Headers included in the request
    std::string body;    // Body of the request
};


#endif // REQUEST_H

