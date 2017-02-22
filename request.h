#ifndef REQUEST_H
#define REQUEST_H

#include <utility>


// Represents an HTTP Request
//
// Usage:
//   auto request = Request::Parse(raw_request);
class Request {
public:

    // Parser results from the Parse method
    enum Result {
        good,
        bad,
        indeterminate
    };

    // Default constructor
    Request();

    // Returns the value for the given header or the empty string
    std::string FindHeaderValue(const std::string& name) const;

    // Parses data from the client. The enum return value is good when a
    // complete request has been parsed, bad if the data is invalid, and
    // indeterminate when more data is required. The InputIterator return value
    // indicates how much of the input has been consumed
    template <typename InputIterator>
    std::tuple<Result, InputIterator> Parse(InputIterator begin,
    InputIterator end) {
        while (begin != end) {
            Result result = Consume(*begin++);
            if (result == good || result == bad)
                return std::make_tuple(result, begin);
        }
        return std::make_tuple(indeterminate, begin);
    }

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

protected:

    // Handles the next character of input to the parser
    Result Consume(char input);

private:

    // State used internally by the parser
    enum {
        _method_start,
        _method,
        _uri,
        _http_version_h,
        _http_version_t_1,
        _http_version_t_2,
        _http_version_p,
        _http_version_slash,
        _http_version_major_start,
        _http_version_major,
        _http_version_minor_start,
        _http_version_minor,
        _expecting_newline_1,
        _header_line_start,
        _header_lws,
        _header_name,
        _space_before_header_value,
        _header_value,
        _expecting_newline_2,
        _expecting_newline_3,
        _body
    } state;

    unsigned long long remaining; // Used to track remaining characters to parse

    std::string raw;     // The entire, unparsed request as a string
    std::string method;  // Indicates what is to be performed
    std::string uri;     // Identifies the resource for the request
    std::string version; // HTTP version the requester is using
    Headers     headers; // Headers included in the request
    std::string body;    // Body of the request
};


#endif // REQUEST_H

