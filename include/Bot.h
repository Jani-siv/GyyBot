#pragma once
#include <iostream>
#include <strings.h>
#include "../include/FileHandle.h"
#include "../include/Sock.h"
#include "../include/Obs.h"
#include "../include/Json.h"
#include <map>
class Bot : public Json {
    public:
        Bot();
        ~Bot();
        void runBot(std::string settingsFile);
    private:
        void updateScenes();
        std::string showScenes();
        std::map<int,std::string> scenes; // 0 scene is current
        int obsSocketFd = 0;
        int twitchSocketFd = 0;
        bool botRunning = true;
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
        void changeScene(std::string scene);
        void parseMessages(std::string message);
        bool checkUserPermission(std::string username, std::string userCommand);
        void executeCommand(std::string commandMsg, std::string userCommand);
        std::map<std::string,int> permission;
        std::string owner;
        void addUser(std::string username, std::string permission);
        void showCommands();
        void getScenes(int socketFd);
        void availableRequest();
        void instantReplay();
        Obs obs;
};


