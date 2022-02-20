#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
class FileHandle{
    public:
        FileHandle();
        ~FileHandle();
        struct data{
            std::string Oauth;
            std::string username;
            std::string server;
            unsigned short port;
        };

        struct data settings;
        void getDataFromFile(std::string filename);
    private:
        std::string returnAfterChr(std::string str, char a);
        int openFile(std::string filename);
        void parseSettingsFile(std::string str);
    protected:
};

