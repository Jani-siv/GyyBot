#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <vector>
#define BUFFER 65535

#define ENDLINE "\r\n"
class Sock{
    public:
        Sock();
        ~Sock();
        int initSocket(std::string address, unsigned short port);
        int sendData(std::string data,int socketFd);
        int readWebSock(int socketFd,std::vector<std::string>&dataStr);
        std::string getData(int socketFd);
        struct webSocket{
            char header[2];
            char* payload = nullptr;
        };
        struct connectionData{
            std::string address;
            int port;
        };
        int sendWebSock(std::string payload, int socketFd);
    private:
        std::string convertCharToString(char *a);
        connectionData connData;
        int createWebSocket();
        void closeWebSocket(int socketFd);
        struct webSocket web;
        char buff[BUFFER];
        void getAddressByHost(std::string address);
        std::string ip_addr;
        struct sockaddr_in server_addr;
        struct hostent *server;
        void getSocketFd();
        std::string address;
        int port=0;
        int sockFd=0;

};
