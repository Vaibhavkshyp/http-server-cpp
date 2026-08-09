#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP
#include <unordered_map>
#include <string>

struct HttpResponse
{
    std::string version = "HTTP/1.1";
    int statusCode;
    std::string statusMessage;
    std::unordered_map<std::string,std::string> headers;
    std::string body;

    std::string serialize();
};


#endif 