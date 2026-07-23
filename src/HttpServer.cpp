#include "HttpServer.h"
#include "./utils/RequestParser.cpp"
#include "./utils/ResponseBuilder.cpp"
#include "./utils/FileManager.cpp"
#include "./utils/ResponseSender.cpp"
#include <iostream>
#include <thread>
using namespace std;

HttpServer::HttpServer(int port)
{
    this->port = port;
}

bool initialize()
{
    WSAData wsadata;
    return WSAStartup(MAKEWORD(2, 2), &wsadata);
}

bool HttpServer::init()
{
    // intialize winsock using wsastartup()
    if (initialize())
    {
        cout << "winsock initialization failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return false;
    }
    else
        cout << "winsock initialized" << endl;

    // create a socket
    this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        cout << "socket creation failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return false;
    }
    else
        cout << "socket created" << endl;

    // bind the socket
    sockaddr_in serverAdd;
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &serverAdd.sin_addr);

    if (bind(serverSocket, (sockaddr *)&serverAdd, sizeof(serverAdd)) == SOCKET_ERROR)
    {
        cout << "bind failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }
    else
        cout << "bind successful" << endl;

    // listen for the connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        cout << "listening failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }
    else
    {
        cout << "listening on Port: " << port << endl;
    }
    return true;
}

void HttpServer::run()
{
    // accept the connection
    while (1)
    {
        SOCKET client = accept(serverSocket, nullptr, nullptr);
        if (client == INVALID_SOCKET)
        {
            cout << "accept failed: " << WSAGetLastError() << endl;
            break;
        }
        thread t(&handleClient,this, client);
        t.detach();
    }

    // close socket and deregister itself from dll
    closesocket(serverSocket);
    WSACleanup();
}

void HttpServer::handleClient(SOCKET client)
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

    // parsing the http request
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
    int bytesSent = sendResponse(client,response);
    if (bytesSent == SOCKET_ERROR)
    {
        cout << "Send failed" << endl;
        closesocket(client);
        return;
    }

    // close the socket
    closesocket(client);
}