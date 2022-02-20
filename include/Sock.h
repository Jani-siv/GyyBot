#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

class Sock{
    public:
        Sock();
        ~Sock();
        void initSocket(std::string address, unsigned short port);
        void sendData(std::string data);
    private:
        void getAddressByHost(std::string address);
        std::string ip_addr;
        std::string getData();
        struct sockaddr_in server_addr;
        struct hostent *server;
        void getSocketFd();
        std::string address;
        int port=0;
        int sockFd=0;

};
