#include "gtest/gtest.h"
#include "http_response.h"


class HttpResponseDefaultResponseTest : public ::testing::Test {
protected:

    void VerifyContents(http::response::status_code status) {
        r = http::response::default_response(status);

	    ASSERT_EQ(status, r.status);
	    ASSERT_EQ(http::default_responses::to_string(r.status), r.content);
	    ASSERT_EQ(2, r.headers.size());
	    EXPECT_EQ("Content-Length", r.headers[0].name);
	    EXPECT_EQ(std::to_string(r.content.size()), r.headers[0].value);
	    EXPECT_EQ("Content-Type", r.headers[1].name);
	    EXPECT_EQ("text/html", r.headers[1].value);
    }

    http::response r;
};


TEST_F(HttpResponseDefaultResponseTest, OkStatus) {
	VerifyContents(http::response::status_code::ok);
}


TEST_F(HttpResponseDefaultResponseTest, CreatedStatus) {
	VerifyContents(http::response::status_code::created);
}


TEST_F(HttpResponseDefaultResponseTest, AcceptedStatus) {
	VerifyContents(http::response::status_code::accepted);
}


TEST_F(HttpResponseDefaultResponseTest, NoContentStatus) {
	VerifyContents(http::response::status_code::no_content);
}


TEST_F(HttpResponseDefaultResponseTest, MovedPermanentlyStatus) {
	VerifyContents(http::response::status_code::moved_permanently);
}


TEST_F(HttpResponseDefaultResponseTest, MovedTemporarilyStatus) {
	VerifyContents(http::response::status_code::moved_temporarily);
}


TEST_F(HttpResponseDefaultResponseTest, NotModifiedStatus) {
	VerifyContents(http::response::status_code::not_modified);
}


TEST_F(HttpResponseDefaultResponseTest, BadRequestStatus) {
	VerifyContents(http::response::status_code::bad_request);
}


TEST_F(HttpResponseDefaultResponseTest, UnauthorizedStatus) {
	VerifyContents(http::response::status_code::unauthorized);
}


TEST_F(HttpResponseDefaultResponseTest, ForbiddenStatus) {
	VerifyContents(http::response::status_code::forbidden);
}


TEST_F(HttpResponseDefaultResponseTest, NotFoundStatus) {
	VerifyContents(http::response::status_code::not_found);
}


TEST_F(HttpResponseDefaultResponseTest, InternalServerErrorStatus) {
	VerifyContents(http::response::status_code::internal_server_error);
}


TEST_F(HttpResponseDefaultResponseTest, NotImplementederrorStatus) {
	VerifyContents(http::response::status_code::not_implemented);
}


TEST_F(HttpResponseDefaultResponseTest, BadGatewayStatus) {
	VerifyContents(http::response::status_code::bad_gateway);
}


TEST_F(HttpResponseDefaultResponseTest, ServiceUnavailableStatus) {
	VerifyContents(http::response::status_code::service_unavailable);
}


TEST(HttpResponseTest, html) {
	http::response r = http::response::html_response(
	    http::default_responses::to_string(http::response::status_code::ok));

	ASSERT_EQ(http::response::status_code::ok, r.status);
	ASSERT_EQ(http::default_responses::to_string(http::response::status_code::ok), r.content);
	ASSERT_EQ(2, r.headers.size());
	EXPECT_EQ("Content-Length", r.headers[0].name);
	EXPECT_EQ(std::to_string(r.content.size()), r.headers[0].value);
	EXPECT_EQ("Content-Type", r.headers[1].name);
	EXPECT_EQ("text/html", r.headers[1].value);
}


TEST(HttpResponseTest, plain) {
	http::response r = http::response::plain_text_response(
	    http::default_responses::to_string(http::response::status_code::ok));

	ASSERT_EQ(http::response::status_code::ok, r.status);
	ASSERT_EQ(http::default_responses::to_string(http::response::status_code::ok), r.content);
	ASSERT_EQ(2, r.headers.size());
	EXPECT_EQ("Content-Length", r.headers[0].name);
	EXPECT_EQ(std::to_string(r.content.size()), r.headers[0].value);
	EXPECT_EQ("Content-Type", r.headers[1].name);
	EXPECT_EQ("text/plain", r.headers[1].value);
}


TEST(ToBufferTest, generaltest) {
	http::response r = http::response::plain_text_response(
	    http::default_responses::to_string(http::response::status_code::ok));

	ASSERT_EQ(http::response::status_code::ok, r.status);
	ASSERT_EQ(http::default_responses::to_string(http::response::status_code::ok), r.content);
	ASSERT_EQ(2, r.headers.size());
	EXPECT_EQ("Content-Length", r.headers[0].name);
	EXPECT_EQ(std::to_string(r.content.size()), r.headers[0].value);
	EXPECT_EQ("Content-Type", r.headers[1].name);
	EXPECT_EQ("text/plain", r.headers[1].value);

	std::vector<boost::asio::const_buffer> buffers = r.to_buffers();

	ASSERT_EQ(1 + r.headers.size() * 4 + 2, buffers.size());
}

