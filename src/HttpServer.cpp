#include "HttpServer.hpp"
#include "HttpRequestParser.hpp"
#include "StaticFileHandler.hpp"
#include <iostream>
#include <thread>

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
        std::cout << "winsock initialization failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }
    else
        std::cout << "winsock initialized" << std::endl;

    // create a socket
    this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cout << "socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }
    else
        std::cout << "socket created" << std::endl;

    // bind the socket
    sockaddr_in serverAdd;
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &serverAdd.sin_addr);

    if (bind(serverSocket, (sockaddr *)&serverAdd, sizeof(serverAdd)) == SOCKET_ERROR)
    {
        std::cout << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }
    else
        std::cout << "bind successful" << std::endl;

    // listen for the connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "listening failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }
    else
    {
        std::cout << "listening on Port: " << port << std::endl;
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
            std::cout << "accept failed: " << WSAGetLastError() << std::endl;
            break;
        }
        std::thread t(&handleClient,this, client);
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
        std::cout << "recv failed" << WSAGetLastError() << std::endl;
        closesocket(client);
        return;
    }
    std::string msg(buff, bytesrecv);
    std::cout << "=====================\n";
    std::cout << msg << '\n';
    std::cout << "=====================\n";

    // parsing, handling the http request and sending the http response
    HttpRequest req;
    HttpResponse res;
    if (!parser.parse(msg, req))
    {
        res.statusCode = 400;
        res.statusMessage = "Bad Request";
        res.body = "<h1>400 Bad Request</h1>";
        res.headers["Content-Type"] = "text/html";
        res.headers["Content-Length"] = std::to_string(res.body.size());

        std::string response = res.serialize();
        send(client, response.c_str(), response.size(), 0);

        closesocket(client);
        return;
    }

    fileHandler.handleRequest(req, res);

    std::string response = res.serialize();
    sendAll(client, response);

    // close the socket
    closesocket(client);
};

bool HttpServer::sendAll(SOCKET client, const std::string &response)
{
    size_t totalSent = 0;
    size_t totalSize = response.size();

    while (totalSent < totalSize)
    {
        int bytesSent = send(
            client,
            response.c_str() + totalSent,
            static_cast<int>(totalSize - totalSent),
            0);

        if (bytesSent == SOCKET_ERROR)
            return false;

        totalSent += bytesSent;
    }

    return true;
}