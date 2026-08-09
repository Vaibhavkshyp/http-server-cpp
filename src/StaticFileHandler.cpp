#include "StaticFileHandler.hpp"
#include <fstream>
#include <sstream>
#include "constants.hpp"

bool StaticFileHandler::handleRequest(const HttpRequest &req, HttpResponse &res)
{
    std::string path;

    if (req.path == "/")
        path = "./public/index.html";
    else
        path = "./public" + req.path;

    size_t pos = path.find_last_of('.');
    std::string extension = path.substr(pos);

    std::string content = readFile(path);
    if (content.empty())
    {
        res.statusCode = 404;
        res.statusMessage = "Not Found";
        res.body = "<h1>404 Not Found</h1>";
        res.headers["content-type"] = "text/html";
    }
    else{
        res.statusCode = 200;
        res.statusMessage = "OK";
        res.body = content;
        auto it = mimeTypes.find(extension);
        if (it != mimeTypes.end())
            res.headers["content-type"] = it->second;
        else
            res.headers["content-type"] = "application/octet-stream";
    }
    
    res.headers["content-length"] = std::to_string(res.body.size());
    res.headers["connection"] = "close";
    return true;
};

std::string StaticFileHandler::readFile(const std::string &filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
        return "";
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
};
