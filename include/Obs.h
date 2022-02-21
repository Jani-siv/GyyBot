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
        int sockFd=0;
        Sock conn;
};
