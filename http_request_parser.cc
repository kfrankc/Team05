#include <sstream>
#include "http_request_parser.h"
#include "http_request.h"


namespace http {

// Default constructor
request_parser::request_parser() {
    reset();
}


// Performs a URL-decoding on a string and returns true if succeeded
bool request_parser::decode_url(const std::string& in, std::string& out) {
    out.clear();
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 3 <= in.size()) {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value) {
                    out += static_cast<char>(value);
                    i += 2;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else if (in[i] == '+') {
            out += ' ';
        } else {
            out += in[i];
        }
    }
    return true;
}


// Preps the parser to start on a whole new input
void request_parser::reset() {
    state = method_start;
}


// Parses data
request_parser::result request_parser::consume(request& req,
                                                      char input) {
    req.as_string += input;
    switch (state) {
    case method_start:
        if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
            return bad;
        } else {
            state = method;
            req.method.push_back(input);
            return indeterminate;
        }
    case method:
        if (input == ' ') {
            state = uri;
            return indeterminate;
        } else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
            return bad;
        } else {
            req.method.push_back(input);
            return indeterminate;
        }
    case uri:
        if (input == ' ') {
            state = http_version_h;
            return indeterminate;
        } else if (is_ctl(input)) {
            return bad;
        } else {
            req.uri.push_back(input);
            return indeterminate;
        }
    case http_version_h:
        if (input == 'H') {
            state = http_version_t_1;
            return indeterminate;
        } else {
            return bad;
        }
    case http_version_t_1:
        if (input == 'T') {
            state = http_version_t_2;
            return indeterminate;
        } else {
            return bad;
        }
    case http_version_t_2:
        if (input == 'T') {
            state = http_version_p;
            return indeterminate;
        } else {
            return bad;
        }
    case http_version_p:
        if (input == 'P') {
            state = http_version_slash;
            return indeterminate;
        } else {
            return bad;
        }
    case http_version_slash:
        if (input == '/') {
            state = http_version_major_start;
            return indeterminate;
        } else {
            return bad;
        }
    case http_version_major_start:
        if (is_digit(input)) {
            state = http_version_major;
            return indeterminate;
        } else {
            return bad;
        }
    case http_version_major:
        if (input == '.') {
            state = http_version_minor_start;
            return indeterminate;
        } else if (is_digit(input)) {
            return indeterminate;
        } else {
            return bad;
        }
    case http_version_minor_start:
        if (is_digit(input)) {
            state = http_version_minor;
            return indeterminate;
        } else {
            return bad;
        }
    case http_version_minor:
        if (input == '\r') {
            state = expecting_newline_1;
            return indeterminate;
        } else if (is_digit(input)) {
            return indeterminate;
        } else {
            return bad;
        }
    case expecting_newline_1:
        if (input == '\n') {
            state = header_line_start;
            return indeterminate;
        } else {
            return bad;
        }
    case header_line_start:
        if (input == '\r') {
            state = expecting_newline_3;
            return indeterminate;
        } else if (!req.headers.empty() && (input == ' ' || input == '\t')) {
            state = header_lws;
            return indeterminate;
        } else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
            return bad;
        } else {
            req.headers.push_back(header());
            req.headers.back().name.push_back(input);
            state = header_name;
            return indeterminate;
        }
    case header_lws:
        if (input == '\r') {
            state = expecting_newline_2;
            return indeterminate;
        } else if (input == ' ' || input == '\t') {
            return indeterminate;
        } else if (is_ctl(input)) {
            return bad;
        } else {
            state = header_value;
            req.headers.back().value.push_back(input);
            return indeterminate;
        }
    case header_name:
        if (input == ':') {
            state = space_before_header_value;
            return indeterminate;
        } else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
            return bad;
        } else {
            req.headers.back().name.push_back(input);
            return indeterminate;
        }
    case space_before_header_value:
        if (input == ' ') {
            state = header_value;
            return indeterminate;
        } else {
            return bad;
        }
    case header_value:
        if (input == '\r') {
            state = expecting_newline_2;
            return indeterminate;
        } else if (is_ctl(input)) {
            return bad;
        } else {
            req.headers.back().value.push_back(input);
            return indeterminate;
        }
    case expecting_newline_2:
        if (input == '\n') {
            state = header_line_start;
            return indeterminate;
        } else {
            return bad;
        }
    case expecting_newline_3:
        if (input == '\n' && decode_url(req.uri, req.path) &&
            !(req.path.empty() || req.path[0] != '/' ||
            req.path.find("..") != std::string::npos)) {
            // If path ends in slash (i.e. is a directory) then add "index.html"
            if (req.path[req.path.size() - 1] == '/') req.path += "index.html";

            return good;
        } else {
            return bad;
        }
    default:
        return bad;
    }
}


// Helper function for the parser that checks if c is a character
bool request_parser::is_char(int c) {
    return c >= 0 && c <= 127;
}


// Helper function for the parser that checks if c is a control character
bool request_parser::is_ctl(int c) {
    return (c >= 0 && c <= 31) || (c == 127);
}

// Helper function for the parser that checks if c is a HTTP tspecial
bool request_parser::is_tspecial(int c) {
    switch (c) {
    case '(': case ')': case '<': case '>': case '@':
    case ',': case ';': case ':': case '\\': case '"':
    case '/': case '[': case ']': case '?': case '=':
    case '{': case '}': case ' ': case '\t':
        return true;
    default:
        return false;
    }
}

// Helper function for the parser that checks if c is a digit
bool request_parser::is_digit(int c) {
    return c >= '0' && c <= '9';
}

} // namespace http

