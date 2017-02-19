// Represents an HTTP Request.
//
// Usage:
//   auto request = Request::Parse(raw_request);

#ifndef REQUEST_H
#define REQUEST_H




class Request {
 public:
  static std::unique_ptr<Request> Parse(const std::string& raw_request);

  std::string raw_request() const;
  std::string method() const;
  std::string uri() const;
  std::string version() const;

  using Headers = std::vector<std::pair<std::string, std::string>>;
  Headers headers() const;

  std::string body() const;
};


#endif // REQUEST_H
