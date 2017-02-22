#include "request_handler.h"

// Map used to keep track of all request handlers by name
std::map<std::string, RequestHandler* (*)(void)>* request_handler_builders = nullptr;


// Registers a request handler by name
RequestHandler* RequestHandler::CreateByName(const char* type) {
    const auto type_and_builder = request_handler_builders->find(type);
    if (type_and_builder == request_handler_builders->end()) {
        return nullptr;
    }
    return (*type_and_builder->second)();
}

void RequestHandler::SetUriPrefix(const std::string& prefix) {
    request_uri_prefix = prefix;
}
void RequestHandler::SetConfig(const NginxConfig& config) {
    request_config = config;
}

    
std::string RequestHandler::GetUriPrefix() const {
    return request_uri_prefix;
}
NginxConfig RequestHandler::GetConfig() const {
    return request_config;
}
