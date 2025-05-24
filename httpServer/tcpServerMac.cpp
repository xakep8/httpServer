//
//  tcpServerMac.cpp
//  httpServer
//
//  Created by Kunal Dubey on 24/05/25.
//

#include "tcpServerMac.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <errno.h>

namespace {
const int BUFFER_SIZE=30720;
void log(const std::string &message){
    std::cout<< message <<std::endl;
}

void exitWithError(const std::string &errorMessage){
    int error = errno;
    const std::string systemError = strerror(error);
    std::cout<< systemError << std::endl;
    log("ERROR: " + errorMessage + "-" + systemError);
    exit(1);
}
}

namespace http {
TCPServer::TCPServer(std::string ipAddress, int port):mIpAddress(ipAddress),mPort(port),mSocket(),mNewSocket(),mIncomingMessage(),mSocketAddress(),mSocketAddressLen(sizeof(mSocketAddress)),mServerMessage(buildResponse()){
    mSocketAddress.sin_family=AF_INET;
    mSocketAddress.sin_port=htons(mPort);
    mSocketAddress.sin_addr.s_addr = inet_addr(mIpAddress.c_str());
    startServer();
}
TCPServer::~TCPServer(){
    closeServer();
}

int TCPServer::startServer(){
    mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mSocket<0){
        exitWithError("Cannot create socket");
        return 1;
    }
    if(bind(mSocket, (sockaddr *)&mSocketAddress, mSocketAddressLen)<0){
        exitWithError("Cannot connect socket to address");
        return 1;
    }
    return 0;
}

void TCPServer::startListen(){
    if(listen(mSocket, 20)<0){
        exitWithError("Socket listen failed");
    }
    std::ostringstream ss;
    ss<< "\n*** Listening on Address: "<< inet_ntoa(mSocketAddress.sin_addr)<<"Port: "<<ntohs(mSocketAddress.sin_port)<<"***\n\n";
    log(ss.str());
    
    long int bytesReceived;
    
    while(true){
        log("========= Waiting for a new connection ============\n\n\n");
        acceptConnection(mNewSocket);
        
        char buffer[BUFFER_SIZE] = {0};
        bytesReceived = read(mNewSocket,buffer,BUFFER_SIZE);
        if(bytesReceived<0){
            exitWithError("Failed to read bytes from client socket connection");
        }
        std::ostringstream ss;
        ss<<"----------------- Received Request from client -------------------------\n\n";
        log(ss.str());
        
        sendResponse();
        close(mNewSocket);
    }
}

void TCPServer::acceptConnection(int &new_socket){
    new_socket = accept(mSocket, (sockaddr *)&mSocketAddress, &mSocketAddressLen);
    if(new_socket<0){
        std::ostringstream ss;
        ss<<"Server failed to accept incoming connection from Address: "<<inet_ntoa(mSocketAddress.sin_addr)<<"Port: "<<ntohs(mSocketAddress.sin_port);
        exitWithError(ss.str());
    }
}

std::string TCPServer::buildResponse(){
    std::string htmlFile="<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
    std::ostringstream ss;
    ss<<"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "<<htmlFile.size()<<"\n\n"<<htmlFile;
    return ss.str();
}

void TCPServer::sendResponse(){
    long int bytesSent;
    bytesSent=write(mNewSocket, mServerMessage.c_str(), mServerMessage.size());
    if(bytesSent==mServerMessage.size()){
        log("---------- Server Response sent to client -------------\n\n");
    }
    else{
        log("Error sending response to client");
    }
}

void TCPServer::closeServer(){
    close(mSocket);
    close(mNewSocket);
    exit(0);
}
}
