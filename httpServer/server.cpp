//
//  server.cpp
//  httpServer
//
//  Created by Kunal Dubey on 24/05/25.
//

#include "tcpServerMac.h"

int main(){
    using namespace http;
    TCPServer server = TCPServer("0.0.0.0",8080);
    server.startListen();
    return 0;
}
