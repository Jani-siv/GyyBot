#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#define BUFFER 512

#define ENDLINE "\r\n"
class Sock{
    public:
        Sock();
        ~Sock();
        int initSocket(std::string address, unsigned short port);
        void sendData(std::string data);
        int readWebSock();
        std::string getData();
        struct webSocket{
            char header[2];
            char* payload = nullptr;
        };
        void sendWebSock(std::string payload);
    private:
        struct webSocket web;
        char buff[512];
        void getAddressByHost(std::string address);
        std::string ip_addr;
        struct sockaddr_in server_addr;
        struct hostent *server;
        void getSocketFd();
        std::string address;
        int port=0;
        int sockFd=0;

};
