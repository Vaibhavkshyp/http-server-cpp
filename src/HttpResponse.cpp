#include "HttpResponse.hpp"
#include <string>
#include <sstream>

// HTTP/1.1 200 OK
// Content-Type: text/html
// Content-Length: 13

// Hello, World!

std::string HttpResponse::serialize() {
    std::ostringstream oss;
    std::string statusLine = version + " " + std::to_string(statusCode) + " " + statusMessage;
    oss << statusLine << "\r\n";
    for(const auto& it : headers){
        oss << it.first << ": " << it.second << "\r\n";
    }
    oss << "\r\n";
    oss << body;
    return oss.str();
}