#include <iostream>
#include "HttpServer.hpp"



int main()
{
    HttpServer server(5000);
    if(!server.init()){
        std::cout<<"Initialization failed"<<std::endl;
        return 1;
    }
    server.run();

    return 0;
}