#include "../include/Sock.h"

Sock::Sock()
{
}
Sock::~Sock()
{
    if(this->sockFd >= 0)
    {
        close(this->sockFd);
    }
}
void Sock::initSocket(std::string address, unsigned short port)
{
    this->getSocketFd();
    this->server_addr.sin_family = AF_INET;
    this->server_addr.sin_port = htons(port);
    if(std::isdigit(address[0]) == 0)
            {
                this->getAddressByHost(address);
                address = this->ip_addr;
                std::cout<<"Trying connect to: "<<address<<std::endl;
            }
    if (inet_pton(AF_INET,address.c_str(),&this->server_addr.sin_addr)<=0)
    {
        std::cout<<address<<std::endl;
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

void Sock::getAddressByHost(std::string address)
{
    std::cout<<"address to convert: "<<address<<std::endl;
    struct hostent *server;
    size_t pos = address.find_last_of('/');
    address = address.substr(pos+1,address.length());
    server = gethostbyname(address.c_str());
    if (server == NULL)
    {
        std::cerr<<"Cannot find host with name"<<std::endl;
        exit(-1);
    }
    else
    {
        struct in_addr **addr_list;
        addr_list = (struct in_addr **) server->h_addr_list;
        char ip[100];
        memset(ip,'a',sizeof(ip));
        for (int j = 0; addr_list[j] != NULL; j++)
        {
        strcpy(ip, inet_ntoa(*addr_list[j]));
        break;
        }
        int i=0;
        while(ip[i] != 'a')
        {
            this->ip_addr += ip[i];
            i++;
        }
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
