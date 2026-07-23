#include <iostream>
#include <sstream>
using namespace std;

struct httpRequest
{
    string method;
    string path;
    string version;
};

httpRequest parseRequest(const string &msg)
{
    size_t pos = msg.find("\r\n");
    string requestLine = msg.substr(0, pos);
    istringstream iss(requestLine);
    httpRequest req;
    iss >> req.method >> req.path >> req.version;
    return req;
}