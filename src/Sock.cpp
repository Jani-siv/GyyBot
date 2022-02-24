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
int Sock::initSocket(std::string address, unsigned short port)
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
    return this->sockFd;
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

int Sock::sendData(std::string data, int socketFd)
{
    data += ENDLINE;
    if (data.compare(0,4,"PASS"))
    {
    std::cout<<data<<std::endl;
    }
  int ret = send(socketFd,data.c_str(),data.size(),0);
  if (ret < 0)
  {
      std::cerr<<"Error sending data"<<std::endl;
  }
  return ret;
}

std::string Sock::getData(int socketFd)
{
    std::string line;
    char buffer[BUFFER];
    char *buf = buffer;
    int len;
    for(len=0;recv(socketFd,buf,1,0);len++,buf++)
    {
        if (*buf=='\n')
        {
            if (*(buf-1)=='\r') 
            {
                for(int i=0; i < len-1; i++)
                {
                line += buffer[i];
                }
                if (line.compare(0,4,"PING") == 0)
                    {
                        this->sendData("PONG",socketFd);
                    }
                line += '\n';
                return line;
            }
        }
    }
    return line;
}

int Sock::readWebSock(int socketFd)
{
    char *ptr = this->buff;
    int headLen = 2;
    int emptyData = 2;
    std::cout<<"reading data from websocket"<<std::endl;
    int ret = read(socketFd,ptr,sizeof(this->buff));
    if (ret<0)
    {
        std::cerr<<"error reading socket"<<std::endl;
    }
    else
    {
        std::cout<<"return value of read is: "<<ret<<std::endl;
    }
    std::cout<<"end of reading"<<std::endl;
    memcpy(this->web.header, ptr,headLen *sizeof(char));
    short int len = (this->web.header[1] & 01111111);
    if (len <= 0)
    {
        std::cout<<"no data in stream."<<std::endl;
        return 0;
    }
    int total = emptyData + headLen;
    memcpy(this->buff, ptr+(total), (5)*sizeof(char));
    std::cout<<this->buff<<std::endl;
    if ((this->web.header[0] & 0x0F) != 1)
    {
        std::cout<<"op code not plain text"<<std::endl;
        return -1;
    }
    else
        return 1;
}

int Sock::sendWebSock(std::string payload, int socketFd)
{
    char header[2];
    char mask[4] = {0x11,0x22,0x33,0x44};
    header[0] = 0x81;
    std::cout<<"data to send: "<<std::endl;
    std::cout<<payload<<std::endl;
    //std::string payload = "Hello";
    char endPacket[1] = {0x00};
    payload += endPacket[0];
    std::string packet;
    for (size_t i=0; i < payload.length(); i++)
    {
        packet += payload[i] ^ mask[i&3];
    }
    std::stringstream ss;
    header[1] = (payload.length() & 0xFF);
    header[1] = header[1] | (1 << 7);
    ss << header[0] << header[1]<<mask[0] <<mask[1]<<mask[2]<<mask[3] << packet;
    payload = ss.str();
    int ret = 0;
    ret = send(socketFd, payload.c_str(), payload.size(),0);
    std::cout<<"return value of send is: "<<ret<<std::endl;
    return ret;
}
