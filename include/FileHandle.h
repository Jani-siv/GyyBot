#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <map>
class FileHandle{
    public:
        FileHandle();
        ~FileHandle();
        struct data{
            std::string Oauth;
            std::string username;
            std::string server;
            std::string channel;
            unsigned short port;
            std::string authfile;
            std::string commandfile;
        };

        struct data settings;
        void getDataFromFile(std::string filename);
        void openCommandsFile(std::map<std::string,std::string>&commands);
    private:
        void testFilesFromSettings(std::string filename);
        std::string returnAfterChr(std::string str, char a);
        int openSettingsFile(std::string filename);
        void parseSettingsFile(std::string str);
    protected:
};

