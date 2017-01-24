# Name of the executable
TARGET=server

# Compiler flags
CXXFLAGS+=-std=c++11 -Wall -Werror

# Linker flags
LDFLAGS+=-lboost_system

# Source files
SRC=main.cc server.cc config_parser.cc http_response.cc

$(TARGET): clean
	$(CXX) -o $@ $(SRC) $(CXXFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)

