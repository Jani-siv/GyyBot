#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>

class FileHandle{
    public:
        FileHandle();
        ~FileHandle();
        struct data{
            std::string Oauth;
            std::string username;
            std::string server;
        };

        void getDataFromFile(std::string filename);
    private:
        int openFile(std::string filename);
        void parseSettingsFile(std::string str);
        struct data *settings;
};

