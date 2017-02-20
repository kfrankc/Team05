// Represents an HTTP response.
//
// Usage:
//   Response r;
//   r.SetStatus(RESPONSE_200);
//   r.SetBody(...);
//   return r.ToString();
//
// Constructed by the RequestHandler, after which the server should call ToString
// to serialize.


#ifndef RESPONSE_H
#define RESPONSE_H

#include <boost/asio.hpp>
#include <string>
#include <vector>

#define name first
#define value second

typedef std::pair<std::string, std::string> header;


class Response {
 public:
  enum ResponseCode {
    // Define your HTTP response codes here.
    ok = 200,
    created = 201,
    accepted = 202,
    no_content = 204,
    moved_permanently = 301,
    moved_temporarily = 302,
    not_modified = 304,
    bad_request = 400,
    unauthorized = 401,
    forbidden = 403,
    not_found = 404,
    internal_server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503
  };

    // Creates a default response for a given status code
  static Response default_response(ResponseCode status);

  // Creates a plain text response for the given text
  static Response plain_text_response(std::string&& text);

  // Creates a text/html response for the given html
  static Response html_response(std::string&& html);

  

  
  void SetStatus(const ResponseCode response_code);
  void AddHeader(const std::string& header_name, const std::string& header_value);
  void SetBody(const std::string& body);
  
  std::string ToString() const;



  using Headers = std::vector<std::pair<std::string, std::string>>;
  Headers GetHeaders() const;
  ResponseCode GetStatus() const;
  std::string GetBody() const;



 private:
  ResponseCode status; // Status code of the http response
  std::vector<std::pair<std::string, std::string>> headers; // Vector of headers for the http response
  std::string content; // Body of http response


};





namespace default_responses {

// Gets default message body for a given status code
std::string to_string(Response::ResponseCode status);

} // namespace default_responses





#endif // RESPONSE_H