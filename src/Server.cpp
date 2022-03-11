#include "../include/Server.h"
#include <cstdlib>
#include <sys/socket.h>

Server::Server(std::string settingsFile)
{
    this->settings = settingsFile;
}
Server::~Server()
{
}
void Server::createServer()
{
    
    char buf[DEFAULT_BUFLEN];
    char *bufptr;
    int addrlen = sizeof(this->address);
    bufptr = buf;
    //create socket
    this->initSocket();
    //accept client
    bool trueClient = false;
    while (trueClient == false)
    {
        this->clientFd = accept(this->serverFd, (struct sockaddr *)&this->address,(socklen_t*)&addrlen);
        if  (this->clientFd < 0)
        {
            std::cerr<<"Error to accept client"<<std::endl;
            exit(EXIT_FAILURE);
        }
        int ret = read(this->clientFd,buf,DEFAULT_BUFLEN);
        std::string hello = "hello";
        for (int i=0; i < hello.length(); i++)
        {
            if (buf[i] != hello[i])
            {
                trueClient = false;
                break;
            }
        trueClient = true;
        }
    if (trueClient == true)
        break;
    }
//create bot
if (trueClient == true)
{
    std::cout<<"Socket number: "<<this->clientFd<<std::endl;
    
    this->bot = new Bot;
    this->bot->serverVersion = true;
    this->bot->runBotServer(this->settings, this->clientFd);

}
//communicate

}

void Server::initSocket()
{
    this->serverFd = socket(AF_INET,SOCK_STREAM,0);
        if (this->serverFd == 0) 
    {
        std::cerr<<"error creating socket"<<std::endl;
        exit(-1);
    }
 //   if (setsockopt(this->serverFd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,
   //             &this->opt,sizeof(this->opt)))
  //  {
    //    std::cerr<<"error setsockopt"<<std::endl;
      //  exit(-1);
   // }
    this->address.sin_family = AF_INET;
   // this->address.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET,"0.0.0.0",&this->address.sin_addr);
    this->address.sin_port = htons(PORT);
    if (bind(this->serverFd, (struct sockaddr *)&this->address,sizeof(this->address))<0)
    {
        std::cerr<<"error to bind socket"<<std::endl;
        exit(-1);
    }
    if (listen(this->serverFd,3) < 0)
    {
        std::cerr<<"unable lister server fd"<<std::endl;
        exit(-1);
    }
}

