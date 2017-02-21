#include "gtest/gtest.h"
#include "http_handler_echo.h"
#include "http_request.h"
#include "http_response.h"


class HttpHandlerEchoTest : public ::testing::Test {
protected:
    void form_request(std::string path) {
        r.path = path; 
        r.method = "GET";
        r.uri = "/";       
    }
    
    http::request r;

};


TEST_F(HttpHandlerEchoTest, SimpleEcho) {

    
    http::handler_echo hf("/echo");
    form_request("/echo/Makefile");

    Response res1 = hf.handle_request(r);

    Response res2 = Response::plain_text_response(std::string(r.as_string));

    EXPECT_EQ(res1.GetHeaders()[0].name, res1.GetHeaders()[0].name);
    EXPECT_EQ(res2.GetHeaders()[0].value, res1.GetHeaders()[0].value);
    EXPECT_EQ(res2.GetHeaders()[1].name, res1.GetHeaders()[1].name);
    EXPECT_EQ(res2.GetHeaders()[1].value, res1.GetHeaders()[1].value);
    EXPECT_EQ(res2.GetBody(), res1.GetBody());
    EXPECT_EQ(res2.GetStatus(), res1.GetStatus());
    



}

TEST_F(HttpHandlerEchoTest, InvalidBaseUrl) {

    form_request("");
    http::handler_echo hf("/echo");

    Response res = hf.handle_request(r);

    EXPECT_EQ(Response::ResponseCode::internal_server_error, res.GetStatus());

    form_request("Makefile");

    res = hf.handle_request(r);
    EXPECT_EQ(Response::ResponseCode::internal_server_error, res.GetStatus());

}