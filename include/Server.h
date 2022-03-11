#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <strings.h>
#include "Bot.h"
#define PORT 5555
#define DEFAULT_BUFLEN 65535

class Server{
  public:
      Server(std::string settingsFile);
      ~Server();
      void createServer();
      void runServer();
  private:
      std::string settings;
      void initSocket();
      int serverFd=0;
      int clientFd = 0;
      struct sockaddr_in address;
      int opt =1;
      Bot* bot;
};
