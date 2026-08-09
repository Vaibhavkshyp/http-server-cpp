#include "HttpRequestParser.hpp"
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <string>
#include "constants.hpp"


HttpRequestParser::HttpRequestParser() {};
HttpRequestParser::~HttpRequestParser() {};

bool HttpRequestParser::parse(std::string &msg, HttpRequest &httpRequest)
{
    // divide the received string into 3 parts - requestline, header, body

    // Request Line
    size_t requestEnd = msg.find("\r\n");
    if (requestEnd == std::string::npos)
        return false;

    if (!requestLineParser(msg.substr(0, requestEnd), httpRequest))
        return false;

    // Headers
    size_t headerEnd = msg.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
        return false;

    size_t headerStart = requestEnd + 2;

    std::string headers =
        msg.substr(headerStart, headerEnd - headerStart);

    if (!headerParser(headers, httpRequest))
        return false;

    // Body
    std::string body = msg.substr(headerEnd + 4);

    if (!bodyParser(body, httpRequest))
        return false;

    return true;
};

bool HttpRequestParser::requestLineParser(const std::string &requestLine, HttpRequest &httpRequest)
{
    if (requestLine.empty())
        return false;
    size_t firstSpace = requestLine.find(' ');
    if (firstSpace == std::string::npos)
        return false;
    size_t secondSpace = requestLine.find(' ', firstSpace + 1);
    if (secondSpace == std::string::npos)
        return false;

    std::string method = requestLine.substr(0, firstSpace);
    auto it = methods.find(method);
    if (it == methods.end())
        return false;
    httpRequest.method = method;

    std::string version = requestLine.substr(secondSpace + 1);
    it = versions.find(version);
    if (it == versions.end())
        return false;
    httpRequest.version = version;

    std::string pathString = requestLine.substr(firstSpace + 1, secondSpace - firstSpace - 1);
    if (pathString.empty() || pathString[0] != '/')
        return false;
    size_t queryPos = pathString.find('?');
    if (queryPos != std::string::npos)
    {
        queryParamsParser(pathString.substr(queryPos + 1), httpRequest);
        httpRequest.path = pathString.substr(0, queryPos);
    }
    else
    {
        httpRequest.path = pathString;
    }
    return true;
};

bool HttpRequestParser::headerParser(const std::string &headers, HttpRequest &httpRequest)
{
    size_t headerStart = 0;
    size_t headerEnd;
    size_t colonPos;
    std::string headerLine;
    std::string key;
    while (headerStart < headers.size())
    {
        headerEnd = headers.find("\r\n", headerStart);
        if (headerEnd == std::string::npos)
        {
            headerLine = headers.substr(headerStart);
            headerStart = headers.size();
        }
        else
        {
            headerLine = headers.substr(headerStart, headerEnd - headerStart);
            headerStart = headerEnd + 2;
        }
        colonPos = headerLine.find(':');
        if (colonPos == std::string::npos)
            return false;
        std::string value = headerLine.substr(colonPos + 1);
        if (!value.empty() && value[0] == ' ')
            value.erase(0, 1);
        key = headerLine.substr(0, colonPos);
        if (key.empty())
            return false;

        std::transform(key.begin(), key.end(), key.begin(),
                       [](unsigned char c)
                       {
                           return std::tolower(c);
                       });
        httpRequest.headers[key] = value;
    }
    return true;
};

bool HttpRequestParser::bodyParser(const std::string &body, HttpRequest &httpRequest)
{
    if ((httpRequest.method == "GET" || httpRequest.method == "HEAD") &&
        !body.empty())
    {
        return false;
    }

    auto it = httpRequest.headers.find("content-length");

    if (it == httpRequest.headers.end())
    {
        if (!body.empty())
            return false;

        httpRequest.body.clear();
        return true;
    }

    size_t bytes;

    try
    {
        bytes = std::stoul(it->second);
    }
    catch (const std::exception &)
    {
        return false;
    }

    if (bytes != body.size())
        return false;

    httpRequest.body = body;

    return true;
}

bool HttpRequestParser::queryParamsParser(const std::string &queryParams, HttpRequest &httpRequest)
{
    if (queryParams.empty())
        return true;
    std::string query = queryParams;
    size_t end;
    while (!query.empty())
    {
        end = query.find('&');
        if (end == std::string::npos)
        {
            if (!storeQueryParams(query, httpRequest))
                return false;
            break;
        }
        storeQueryParams(query.substr(0, end), httpRequest);
        query = query.substr(end + 1);
    }
    return true;
}

bool HttpRequestParser::storeQueryParams(const std::string &query, HttpRequest &httpRequest)
{
    size_t pos = query.find('=');
    if (pos == std::string::npos)
        return false;
    httpRequest.queryParams[query.substr(0, pos)] = query.substr(pos + 1);
    return true;
}
