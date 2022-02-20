#include <iostream>
#include <strings.h>
#include "../include/FileHandle.h"
#include "../include/Sock.h"
class Bot {
    public:
        Bot();
        ~Bot();
        void runBot(std::string settingsFile);
    private:
        FileHandle handle;
        Sock connection;
};


