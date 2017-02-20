#include "gtest/gtest.h"
#include "response.h"


class ResponseDefaultResponseTest : public ::testing::Test {
protected:

    void VerifyContents(Response::ResponseCode status) {
        r = Response::default_response(status);

        ASSERT_EQ(status, r.status);
        ASSERT_EQ(default_responses::to_string(r.status), r.content);
        ASSERT_EQ(2, r.headers.size());
        EXPECT_EQ("Content-Length", r.headers[0].name);
        EXPECT_EQ(std::to_string(r.content.size()), r.headers[0].value);
        EXPECT_EQ("Content-Type", r.headers[1].name);
        EXPECT_EQ("text/html", r.headers[1].value);
    }

    Response r;
};


TEST_F(ResponseDefaultResponseTest, OkStatus) {
    VerifyContents(Response::ResponseCode::ok);
}


TEST_F(ResponseDefaultResponseTest, CreatedStatus) {
    VerifyContents(Response::ResponseCode::created);
}


TEST_F(ResponseDefaultResponseTest, AcceptedStatus) {
    VerifyContents(Response::ResponseCode::accepted);
}


TEST_F(ResponseDefaultResponseTest, NoContentStatus) {
    VerifyContents(Response::ResponseCode::no_content);
}


TEST_F(ResponseDefaultResponseTest, MovedPermanentlyStatus) {
    VerifyContents(Response::ResponseCode::moved_permanently);
}


TEST_F(ResponseDefaultResponseTest, MovedTemporarilyStatus) {
    VerifyContents(Response::ResponseCode::moved_temporarily);
}


TEST_F(ResponseDefaultResponseTest, NotModifiedStatus) {
    VerifyContents(Response::ResponseCode::not_modified);
}


TEST_F(ResponseDefaultResponseTest, BadRequestStatus) {
    VerifyContents(Response::ResponseCode::bad_request);
}


TEST_F(ResponseDefaultResponseTest, UnauthorizedStatus) {
    VerifyContents(Response::ResponseCode::unauthorized);
}


TEST_F(ResponseDefaultResponseTest, ForbiddenStatus) {
    VerifyContents(Response::ResponseCode::forbidden);
}


TEST_F(ResponseDefaultResponseTest, NotFoundStatus) {
    VerifyContents(Response::ResponseCode::not_found);
}


TEST_F(ResponseDefaultResponseTest, InternalServerErrorStatus) {
    VerifyContents(Response::ResponseCode::internal_server_error);
}


TEST_F(ResponseDefaultResponseTest, NotImplementederrorStatus) {
    VerifyContents(Response::ResponseCode::not_implemented);
}


TEST_F(ResponseDefaultResponseTest, BadGatewayStatus) {
    VerifyContents(Response::ResponseCode::bad_gateway);
}


TEST_F(ResponseDefaultResponseTest, ServiceUnavailableStatus) {
    VerifyContents(Response::ResponseCode::service_unavailable);
}


TEST(ResponseTest, html) {
    Response r = Response::html_response(
        default_responses::to_string(Response::ResponseCode::ok));

    ASSERT_EQ(Response::ResponseCode::ok, r.status);
    ASSERT_EQ(default_responses::to_string(Response::ResponseCode::ok), r.content);
    ASSERT_EQ(2, r.headers.size());
    EXPECT_EQ("Content-Length", r.headers[0].name);
    EXPECT_EQ(std::to_string(r.content.size()), r.headers[0].value);
    EXPECT_EQ("Content-Type", r.headers[1].name);
    EXPECT_EQ("text/html", r.headers[1].value);
}


TEST(ResponseTest, plain) {
    Response r = Response::plain_text_response(
        default_responses::to_string(Response::ResponseCode::ok));

    ASSERT_EQ(Response::ResponseCode::ok, r.status);
    ASSERT_EQ(default_responses::to_string(Response::ResponseCode::ok), r.content);
    ASSERT_EQ(2, r.headers.size());
    EXPECT_EQ("Content-Length", r.headers[0].name);
    EXPECT_EQ(std::to_string(r.content.size()), r.headers[0].value);
    EXPECT_EQ("Content-Type", r.headers[1].name);
    EXPECT_EQ("text/plain", r.headers[1].value);
}


TEST(ToBufferTest, generaltest) {
    Response r = Response::plain_text_response(
        default_responses::to_string(Response::ResponseCode::ok));

    ASSERT_EQ(Response::ResponseCode::ok, r.status);
    ASSERT_EQ(default_responses::to_string(Response::ResponseCode::ok), r.content);
    ASSERT_EQ(2, r.headers.size());
    EXPECT_EQ("Content-Length", r.headers[0].name);
    EXPECT_EQ(std::to_string(r.content.size()), r.headers[0].value);
    EXPECT_EQ("Content-Type", r.headers[1].name);
    EXPECT_EQ("text/plain", r.headers[1].value);

    std::vector<boost::asio::const_buffer> buffers = r.to_buffers();

    ASSERT_EQ(1 + r.headers.size() * 4 + 2, buffers.size());
}


TEST(ToStringTest, generaltest) {
    Response r = Response::plain_text_response(
        default_responses::to_string(Response::ResponseCode::ok));

    ASSERT_EQ(Response::ResponseCode::ok, r.status);
    ASSERT_EQ(default_responses::to_string(Response::ResponseCode::ok), r.content);
    ASSERT_EQ(2, r.headers.size());
    EXPECT_EQ("Content-Length", r.headers[0].name);
    EXPECT_EQ(std::to_string(r.content.size()), r.headers[0].value);
    EXPECT_EQ("Content-Type", r.headers[1].name);
    EXPECT_EQ("text/plain", r.headers[1].value);


    EXPECT_EQ("HTTP/1.0 200 OK\r\nContent-Length: 0\r\nContent-Type: text/plain\r\n\r\n", r.ToString());
}

