#include <iostream>
#include "HttpServer.h"

using namespace std;

#pragma comment(lib, "ws2_32.lib") // link with winsock lib

int main()
{
    HttpServer server(5000);
    if(!server.init()){
        cout<<"Initialization failed"<<endl;
        return 1;
    }
    server.run();

    return 0;
}