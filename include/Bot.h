#include <iostream>
#include <strings.h>
#include "FileHandle.h"

class Bot {
    public:
        Bot();
        ~Bot();
        void runBot(std::string settingsFile);
    private:
        FileHandle handle;
};


