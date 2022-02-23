#pragma once
#include <iostream>
#include <strings.h>
#include "../include/FileHandle.h"
#include "../include/Sock.h"
#include "../include/Obs.h"
#include <map>
class Bot {
    public:
        Bot();
        ~Bot();
        void runBot(std::string settingsFile);
    private:
        std::map<std::string,std::string>commands; //command, permission
        std::map<std::string,std::string>users;
        FileHandle handle;
        Sock connection;
        void authenticate();
        void joinChannel();
        void leaveChannel();
        void sendPrivMsg(std::string payload);
        std::string lowerCase(std::string data);
        void listenBroadCast();
        void changeScene();
        void parseMessages(std::string message);
        bool checkUserPermission(std::string username, std::string userCommand);
        std::map<std::string,int> permission;
        
        Obs obs;
};


