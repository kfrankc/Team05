CC=c++

#link -lboost_system library
CFLAGS = -std=c++0x -g -Wall 

#location of boost library
#INCLUDES = -I /include/boost

LIBS = -lboost_system 

SRCS = async_tcp_echo_server.cc

TARGET = server

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(INCLUDES) $(SRCS) $(CFLAGS) $(LIBS) -o $@ 


clean: 
	  $(RM) $(TARGET)