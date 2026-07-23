#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <winsock2.h> // winsock api
#include <WS2tcpip.h> // tcp ip utilities(inet_pton)
#include <windows.h>  // windows specific APIs (sleep)

class HttpServer
{
public:
    HttpServer(int port);

    bool init();
    void run();

private:
    void handleClient(SOCKET client);

private:
    int port;
    SOCKET serverSocket = INVALID_SOCKET;
};

#endif