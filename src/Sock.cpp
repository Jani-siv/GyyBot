#include "../include/Sock.h"

Sock::Sock()
{
}
Sock::~Sock()
{
}
void Sock::initSocket(std::string address, unsigned short port)
{
    this->getSocketFd();
    this->server_addr.sin_family = AF_INET;
    this->server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET,address.c_str(),&this->server_addr.sin_addr)<=0)
    {
        std::cerr<<"Invalid address"<<std::endl;
        exit(-1);
    }
    if (connect(this->sockFd,(struct sockaddr *)&this->server_addr, sizeof(this->server_addr)) < 0)
    {
        std::cerr<<"Connection failed"<<std::endl;
        exit(-1);
    }
    
    std::cout<<"connected to server"<<std::endl;
}


void Sock::getSocketFd()
{
if ((this->sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
{
    std::cerr<<"Error opening socket"<<std::endl;
    exit(-1);
}
}

void Sock::sendData(std::string data)
{

}

std::string Sock::getData()
{
    std::string test = "";
    return test;
}
