#pragma once
#include <iostream>
#include "Sock.h"
#include <string>
#include <sstream>

class Obs{
    public:
        Obs();
        ~Obs();
        void initConnection(std::string address, int port);
    private:
        std::string connectionCommand(std::string address, int port);
        int sockFd=0;
        Sock conn;
};
