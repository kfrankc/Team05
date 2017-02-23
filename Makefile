# Name of the executable
TARGET=webserver

# Test executables
TESTEXEC=config_parser_test response_test #server_config_parser_test
GCOVEXEC=config_parser_gcov response_gcov #server_config_parser_gcov

# GoogleTest directory and output files
GTEST_DIR=googletest/googletest
GTEST_FILES=libgtest.a gtest-all.o

# Gcov flags and files
GCOVFLAGS=-fprofile-arcs -ftest-coverage
GCOVFILES=*.gcno *.gcda *.gcov

# Compiler flags
CXXFLAGS+=-std=c++11 -Wall -Werror

# Linker flags
LDFLAGS+=-lboost_system

# Test flags
TESTFLAGS=-std=c++11 -isystem ${GTEST_DIR}/include -pthread

# Source files
SRC=main.cc server.cc config_parser.cc response.cc \
server_config_parser.cc request.cc echo_handler.cc \
static_file_handler.cc request_handler.cc \
not_found_handler.cc status_handler.cc

.PHONY: clean clean_target gcov test test_gcov test_setup

$(TARGET): clean_target
	$(CXX) -o $@ $(SRC) $(CXXFLAGS) $(LDFLAGS)

clean_target:
	$(RM) $(TARGET)

clean: clean_target
	$(RM) $(GCOVFILES) $(TESTEXEC) $(GTEST_FILES) *_gcov.txt

test_gcov: $(GCOVEXEC)

test_setup: 
	g++ $(TESTFLAGS) -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

test: $(TARGET) $(TESTEXEC) integration_test

integration_test: $(TARGET)
	python webserver_test.py

config_parser_test: test_setup config_parser.cc config_parser_test.cc
	g++ $(GCOVFLAGS) $(TESTFLAGS) config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o $@ $(LDFLAGS)
	./$@

config_parser_gcov: config_parser_test
	gcov -r config_parser.cc > config_parser_gcov.txt

response_test: test_setup response.cc response_test.cc
	g++ $(GCOVFLAGS) $(TESTFLAGS) response_test.cc response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o $@ $(LDFLAGS)
	./$@

response_gcov: response_test
	gcov -r response.cc > response_gcov.txt	

#server_config_parser_test: test_setup server_config_parser.cc server_config_parser_test.cc
#	g++ $(GCOVFLAGS) $(TESTFLAGS) server_config_parser_test.cc server_config_parser.cc config_parser.cc response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o $@ $(LDFLAGS)
#	./$@

#server_config_parser_gcov: server_config_parser_test
#	gcov -r server_config_parser.cc > server_config_parser_gcov.txt

