GTEST_DIR=googletest/googletest

# Name of the executable
TARGET=webserver

# Compiler flags
CXXFLAGS+=-std=c++11 -Wall -Werror

# Linker flags
LDFLAGS+=-lboost_system

# Source files
SRC=main.cc server.cc config_parser.cc http_response.cc

$(TARGET): clean
	$(CXX) -o $@ $(SRC) $(CXXFLAGS) $(LDFLAGS)

.PHONY: clean test 

clean:
	$(RM) $(TARGET) $(OBJS)

test_setup: 
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc; \
	ar -rv libgtest.a gtest-all.o;

test: test_setup config_parser_test

config_parser_test: test_setup config_parser.cc config_parser_test.cc
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test

http_response_test: test_setup
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread http_response_test.cc http_response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o http_response_test $(LDFLAGS)