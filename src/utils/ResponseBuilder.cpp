#include <iostream>
#include <sstream>
using namespace std;

struct httpResponse
{
    string status;
    string statusText;
    string contentType;
    string body;
};

string buildResponse(const httpResponse &response)
{
    ostringstream oss;
    string statusLine = "HTTP/1.1 " + response.status + " " + response.statusText;
    oss << statusLine << "\r\n";
    oss << "Content-Type: " << response.contentType << "\r\n";
    oss << "Content-Length: " << response.body.size() << "\r\n\r\n";
    oss << response.body;
    string output = oss.str();
    return output;
}