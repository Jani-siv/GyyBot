#pragma once
#include <iostream>
#include "Sock.h"
#include <string>
#include <sstream>
#include <vector>
class Obs{
    public:
        Obs();
        ~Obs();
        int initConnection(std::string address, int port);
        void getScenes(int socketFd);
        void setScene(std::string scene, int socketFd);
        void getAvailableRequest(int socketFd);
        void saveReplay(int socketFd, std::string current);
        std::string getLastRead();
    private:
        std::vector<std::string> dataStorage;
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
