#include <iostream>
#include <sstream>

class FileHandle{
    public:
        FileHandle();
        ~FileHandle();
        void getDataFromFile(std::string filename);
    private:
        int openFile(std::string filename);
        struct data{
            std::string Oauth;
            std::string username;
            std::string server;
        };
};

