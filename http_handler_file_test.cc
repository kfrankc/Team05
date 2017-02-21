#include "gtest/gtest.h"
#include "http_handler_file.h"
#include "http_request.h"
#include "response.h"




class HttpHandlerFileTest : public ::testing::Test {
protected:
    void form_request(std::string path) {
        r.path = path;        
    }
    
    http::request r;

};

TEST_F(HttpHandlerFileTest, FileExists) {

    int fd;
    char name[] = "/tmp/fileXXXXXX";


    std::string text_contents = "foo bar\n bar foo";

    // Creates a temporary file using name, replacing the last 6 X's 
    fd = mkstemp(name);

    // Write the text_contents into the temp file
    write(fd, text_contents.c_str(), text_contents.size());

    form_request("/static1" + std::string(name));
    http::handler_file hf("/", "/static1");


    Response res = hf.handle_request(r);


    //Delete the file
    remove(name);


    EXPECT_EQ("Content-Length", res.GetHeaders()[0].name);
    EXPECT_EQ(std::to_string(text_contents.size()), res.GetHeaders()[0].value);
    EXPECT_EQ("Content-Type", res.GetHeaders()[1].name);
    EXPECT_EQ("text/plain", res.GetHeaders()[1].value);
    EXPECT_EQ(text_contents, res.GetBody());
    EXPECT_EQ(Response::ResponseCode::ok, res.GetStatus());

    
}

TEST_F(HttpHandlerFileTest, WrongHandlerCalled) {

    int fd;
    char name[] = "/tmp/fileXXXXXX";


    std::string text_contents = "foo bar\n bar foo";

    // Creates a temporary file using name, replacing the last 6 X's 
    fd = mkstemp(name);

    // Write the configcontents into the temp file
    write(fd, text_contents.c_str(), text_contents.size());

    form_request("/static11" + std::string(name));
    http::handler_file hf("/", "/static1");


    Response res = hf.handle_request(r);

    // Delete the file
    remove(name);

    EXPECT_EQ("Content-Length", res.GetHeaders()[0].name);
    EXPECT_EQ("Content-Type", res.GetHeaders()[1].name);
    EXPECT_EQ("text/html", res.GetHeaders()[1].value);
    EXPECT_EQ(Response::ResponseCode::internal_server_error, res.GetStatus());
    
}


TEST_F(HttpHandlerFileTest, NoFileAskedFor) {

    form_request("/static1");
    http::handler_file hf("/", "/static1");


    Response res = hf.handle_request(r);


    EXPECT_EQ("Content-Length", res.GetHeaders()[0].name);
    EXPECT_EQ("Content-Type", res.GetHeaders()[1].name);
    EXPECT_EQ(Response::ResponseCode::not_found, res.GetStatus());

}


TEST_F(HttpHandlerFileTest, FileDoesNotExit) {

    form_request("/static1/filethatdoesntexist" );
    http::handler_file hf("/", "/static1");


    Response res = hf.handle_request(r);


    EXPECT_EQ("Content-Length", res.GetHeaders()[0].name);
    EXPECT_EQ("Content-Type", res.GetHeaders()[1].name);
    EXPECT_EQ(Response::ResponseCode::not_found, res.GetStatus());

}

TEST_F(HttpHandlerFileTest, InvalidBaseUrl) {

    form_request("");
    http::handler_file hf("/", "/static1");

    Response res = hf.handle_request(r);

    EXPECT_EQ(Response::ResponseCode::internal_server_error, res.GetStatus());

    form_request("Makefile");

    res = hf.handle_request(r);
    EXPECT_EQ(Response::ResponseCode::internal_server_error, res.GetStatus());

}





