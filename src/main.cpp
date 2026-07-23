#include <iostream>
#include <winsock2.h> // winsock api
#include <WS2tcpip.h> // tcp ip utilities(inet_pton)
#include <windows.h>  // windows specific APIs (sleep)
#include <thread>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

#pragma comment(lib, "ws2_32.lib") // link with winsock lib

const unordered_map<string, string> mimeTypes = {
    {".html", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".png", "image/png"},
    {".jpeg", "image/jpeg"},
    {".jpg", "image/jpeg"},
    {".ico", "image/x-icon"}};

struct httpRequest
{
    string method;
    string path;
    string version;
};

struct httpResponse
{
    string status;
    string statusText;
    string contentType;
    string body;
};

struct fileData
{
    bool success;
    string content;
    string type;
};

bool initialize()
{
    WSAData wsadata;
    return WSAStartup(MAKEWORD(2, 2), &wsadata);
}

httpRequest parseRequest(const string &msg)
{
    size_t pos = msg.find("\r\n");
    string requestLine = msg.substr(0, pos);
    istringstream iss(requestLine);
    httpRequest req;
    iss >> req.method >> req.path >> req.version;
    return req;
}

fileData readFile(const string &fileName)
{
    fileData file;
    file.success = false;
    file.content = "";
    file.type = "application/octet-stream";
    ifstream f(fileName, ios_base::binary);
    if (!f)
    {
        file.type = "text/html";
        return file;
    }
    else
    {
        stringstream buffer;
        buffer << f.rdbuf();
        file.success = true;
        file.content = buffer.str();
        size_t pos = fileName.find_last_of(".");
        if (pos != string::npos)
        {
            string extension = fileName.substr(pos);
            auto it = mimeTypes.find(extension);
            if (it != mimeTypes.end())
                file.type = it->second;
        }
    }
    return file;
}
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
int sendResponse(SOCKET client, const string &response)
{
    int bytesSent = send(client, response.c_str(), response.size(), 0);
    return bytesSent;
}

void handleClient(SOCKET client)
{
    // receive the incoming http request
    char buff[1024];
    int bytesrecv = recv(client, buff, sizeof(buff), 0);
    if (bytesrecv == 0)
    {
        closesocket(client);
        return;
    }
    else if (bytesrecv == SOCKET_ERROR)
    {
        cout << "recv failed" << WSAGetLastError() << endl;
        closesocket(client);
        return;
    }
    string msg(buff, bytesrecv);

    //parsing the http request
    httpRequest req = parseRequest(msg);
    if (req.method.empty() ||
        req.path.empty() ||
        req.version.empty())
    {
        closesocket(client);
        return;
    }

    // reading the file content and saving it to fileData struct
    fileData file;
    if (req.path == "/")
    {
        file = readFile("../public/index.html");
    }
    else
    {
        file = readFile("../public" + req.path);
    }

    // build the response to send
    string response;
    httpResponse responseStatus;
    responseStatus.status = "404";
    responseStatus.statusText = "Not Found";
    responseStatus.contentType = file.type;
    responseStatus.body = "<h1>404 Not Found!</h1>";
    if (file.success)
    {
        responseStatus.status = "200";
        responseStatus.statusText = "OK";
        responseStatus.contentType = file.type;
        responseStatus.body = file.content;
    }
    response = buildResponse(responseStatus);

    // send the http response
    int bytesSent = sendResponse(client, response);
    if (bytesSent == SOCKET_ERROR)
    {
        cout << "Send failed" << endl;
        return;
    }

    //close the socket
    closesocket(client);
}

int main()
{
    // intialize winsock using wsastartup()
    if (initialize())
    {
        cout << "winsock initialization failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }
    else
        cout << "winsock initialized" << endl;

    // create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        cout << "socket creation failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return 2;
    }
    else
        cout << "socket created" << endl;

    // bind the socket
    int port = 5000;
    sockaddr_in serverAdd;
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &serverAdd.sin_addr);

    if (bind(serverSocket, (sockaddr *)&serverAdd, sizeof(serverAdd)) == SOCKET_ERROR)
    {
        cout << "bind failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }
    else
        cout << "bind successful" << endl;

    // listen for the connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        cout << "listening failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }
    else
    {
        cout << "listening on Port: " << port << endl;
    }

    // accept the connection
    while (1)
    {
        SOCKET client = accept(serverSocket, nullptr, nullptr);
        if (client == INVALID_SOCKET)
        {
            cout << "accept failed: " << WSAGetLastError() << endl;
            break;
        }
        thread t(handleClient, client);
        t.detach();
    }

    // close socket and deregister itself from dll
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}