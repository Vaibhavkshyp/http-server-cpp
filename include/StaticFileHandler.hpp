#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <string>

class StaticFileHandler
{
public:
    bool handleRequest(const HttpRequest& req, HttpResponse& res);
private:
    std::string readFile(const std::string &filePath);
};
