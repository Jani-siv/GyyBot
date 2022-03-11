#include <iostream>
#include <strings.h>
#include "../include/Bot.h"
#include "../include/Obs.h"
#include "../include/Server.h"
int main(int argc, char *argv[])
{
if (argc  < 2)
{
    std::cerr<<"Please set config filename"<<"\n";
    exit(-1);
}
    std::cout<<"Hello there.. My name is GyyBot"<<"\n";
    std::cout<<"config filename is: "<<argv[1] <<"\n";
    std::string settingsFile = argv[1];
    Server ser(settingsFile);
ser.createServer();
    //Bot GyyBot;
//std::string configFile =argv[1];
//std::cout<<configFile<<std::endl;
//GyyBot.runBot(configFile);

//Obs test;
//test.initConnection("192.168.0.224",4444);
return 0;
}
