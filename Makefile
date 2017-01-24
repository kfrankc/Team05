#CXX=g++

# compile flags
CXXFLAGS = -std=c++11 -g -Wall -Werror

# location of boost library
#INCLUDES = -I /include/boost -I.
INCLUDES = -I.

# link -lboost_system library
LIBS = -lboost_system 

# .cc files
SRCS = async_tcp_echo_server.cc webserver.cc config_parser.cc

# target to compile
TARGET = webserver

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(INCLUDES) $(SRCS) $(CXXFLAGS) $(LIBS) -o $@ 


clean: 
	$(RM) $(TARGET)