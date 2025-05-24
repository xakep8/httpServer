//
//  tcpServerMac.h
//  httpServer
//
//  Created by Kunal Dubey on 24/05/25.
//

#ifndef INCLUDED_HTTP_TCPSERVER_MAC
#define INCLUDED_HTTP_TCPSERVER_MAC

#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

namespace http {
class TCPServer{
public:
    TCPServer(std::string ipAddress, int port);
    ~TCPServer();
    void startListen();
private:
    std::string mIpAddress;
    int mPort;
    int mSocket;
    int mNewSocket;
    long mIncomingMessage;
    struct sockaddr_in mSocketAddress;
    socklen_t mSocketAddressLen;
    std::string mServerMessage;
    int startServer();
    void closeServer();
    void acceptConnection(int &newSocket);
    std::string buildResponse();
    void sendResponse();
};
}
#endif
