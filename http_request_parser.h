#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include <string>
#include <tuple>


namespace http {

struct request;


// Used to parse data sent from clients into an HTTP request
class request_parser {
public:

    // Results given by the parse method
    enum result {
        good,
        bad,
        indeterminate
    };

    // Default constructor
    request_parser();

    // Performs a URL-decoding on a string and returns true if succeeded
    static bool decode_url(const std::string& in, std::string& out);

    // Parses data. The enum return value is good when a complete request has
    // been parsed, bad if the data is invalid, and indeterminate when more data
    // is required. The InputIterator return value indicates how much of the
    // input has been consumed
    template <typename InputIterator>
    std::tuple<result, InputIterator> parse(request& req,
        InputIterator begin, InputIterator end) {
        while (begin != end) {
            result rslt = consume(req, *begin++);
            if (rslt == good || rslt == bad)
                return std::make_tuple(rslt, begin);
        }
        return std::make_tuple(indeterminate, begin);
    }

    // Preps the parser to start on a whole new input
    void reset();

protected:

    // Handles the next character of input to the parser
    result consume(request& req, char input);

private:

    // State used internally by the parser
    enum parser_state {
        method_start,
        method,
        uri,
        http_version_h,
        http_version_t_1,
        http_version_t_2,
        http_version_p,
        http_version_slash,
        http_version_major_start,
        http_version_major,
        http_version_minor_start,
        http_version_minor,
        expecting_newline_1,
        header_line_start,
        header_lws,
        header_name,
        space_before_header_value,
        header_value,
        expecting_newline_2,
        expecting_newline_3
    };

    // Helper function for the parser that checks if c is a character
    static bool is_char(int c);

    // Helper function for the parser that checks if c is a control character
    static bool is_ctl(int c);

    // Helper function for the parser that checks if c is a HTTP tspecial
    static bool is_tspecial(int c);

    // Helper function for the parser that checks if c is a digit
    static bool is_digit(int c);

private:

    parser_state state; // State the parser is currently in
};

} // namespace http

#endif // HTTP_REQUEST_PARSER_HPP

