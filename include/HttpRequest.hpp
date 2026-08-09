#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <unordered_map>

struct HttpRequest
{
    std::string method;
    std::string path;
    std::string version;
    std::unordered_map<std::string,std::string> headers;
    std::unordered_map<std::string,std::string> queryParams;
    std::string body;
};

#endif
