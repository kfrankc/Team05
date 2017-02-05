#ifndef SERVER_CONFIG_PARSER_H
#define SERVER_CONFIG_PARSER_H

#include "config_parser.h"


// Returns the port number from config_file or -1 if no number is found
//   TODO: Put this in a class as a static method
int port_number(const char* config_file);

#endif // SERVER_CONFIG_PARSER_H

