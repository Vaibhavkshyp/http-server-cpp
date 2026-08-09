#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP
#include "HttpRequest.hpp"
#include <string>

class HttpRequestParser
{
public:
    HttpRequestParser();
    ~HttpRequestParser();
    bool parse(std::string& msg, HttpRequest& httpRequest);

private:
    bool requestLineParser(const std::string &requestLine, HttpRequest &httpRequest);
    bool headerParser(const std::string &headers, HttpRequest &httpRequest);
    bool bodyParser(const std::string &body, HttpRequest &httpRequest);
    bool queryParamsParser(const std::string &queryParams, HttpRequest &httpRequest);
    bool storeQueryParams(const std::string &query,HttpRequest &httpRequest);
};


#endif