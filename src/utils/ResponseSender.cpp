#include <WinSock2.h>
#include <iostream>
using namespace std;

int sendResponse(SOCKET client, const string &response){
    return send(client, response.c_str(), response.size(), 0);
}