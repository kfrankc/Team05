#include <string>
#include <sstream>

#include "gtest/gtest.h"
#include "reverse_proxy_handler.h"
#include "request_handler.h"
#include "request.h"
#include "response.h"
#include "config_parser.h"

// Test fixture for testing RequestProxyHandler
class ReverseProxyHandlerTest : public ::testing::Test {
  protected:
    bool parseString(const std::string config_string) {
      std::stringstream config_stream(config_string);
      return parser.Parse(&config_stream, &out_config);
    }

    RequestHandler::Status initProxy(std::string prefix){
      std::cout << "hello: " << out_config.statements.size() << std::endl;
      return proxy_handler.Init(prefix, out_config);
    }

    std::string getPrefix(){
      return proxy_handler.original_uri_prefix;
    }

    std::string getHost(){
      return proxy_handler.remote_host;
    }

    std::string getPort(){
      return proxy_handler.remote_port;
    }

    RequestHandler::Status HandleRequest(const Request& request, Response* response) {
      return proxy_handler.HandleRequest(request, response);
    }

    NginxConfigParser parser;
    NginxConfig out_config;
    ReverseProxyHandler proxy_handler;
};

TEST_F(ReverseProxyHandlerTest, Init) {
  std::string config_string = "path /reverse_proxy ReverseProxyHandler {"
                              "    remote_host localhost;"
                              "    remote_port 4242;"
                              "}";
  EXPECT_TRUE(parseString(config_string));
  auto init_status = initProxy("/reverse_proxy");
  EXPECT_EQ(init_status, RequestHandler::OK);
  EXPECT_EQ(getPrefix(), "/reverse_proxy");
  EXPECT_EQ(getHost(), "localhost");
  EXPECT_EQ(getPort(), "4242");
}
