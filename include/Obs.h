#pragma once
#include <iostream>
#include "Sock.h"
#include <string>
#include <sstream>

class Obs{
    public:
        Obs();
        ~Obs();
        int initConnection(std::string address, int port);
        void getVersion(int socketFd);
    private:
        int createWebSocket();
        struct connectionData{
            std::string address;
            int port;
        };
        connectionData connData;
        std::string connectionCommand(std::string address, int port);
        int sockFd=0;
        Sock conn;
};
