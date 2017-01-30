GTEST_DIR=googletest/googletest

# Name of the executable
TARGET=webserver

# Compiler flags
CXXFLAGS+=-std=c++11 -Wall -Werror

# Linker flags
LDFLAGS+=-lboost_system

# Source files
SRC=main.cc server.cc config_parser.cc http_response.cc server_config_parser.cc

$(TARGET): clean
	$(CXX) -o $@ $(SRC) $(CXXFLAGS) $(LDFLAGS)

.PHONY: clean test 

clean:
	$(RM) $(TARGET) $(OBJS)

test_setup: 
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc; \
	ar -rv libgtest.a gtest-all.o;

test: test_setup config_parser_test http_response_test server_config_parser_test

config_parser_test: test_setup config_parser.cc config_parser_test.cc
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o $@ 

http_response_test: test_setup http_response.cc http_response_test.cc
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread http_response_test.cc http_response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o $@  $(LDFLAGS)

server_config_parser_test: test_setup server_config_parser.cc server_config_parser_test.cc
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread server_config_parser_test.cc server_config_parser.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o $@ $(LDFLAGS)