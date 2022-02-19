#include <iostream>
#include <strings.h>
#include "../include/Bot.h"
int main(int argc, char *argv[])
{
if (argc  < 2)
{
    std::cerr<<"Please set config filename"<<"\n";
    exit(-1);
}
    std::cout<<"Hello there.. My name is GyyBot"<<"\n";
    std::cout<<"config filename is: "<<argv[1] <<"\n";
Bot GyyBot;
std::string configFile =argv[1];
std::cout<<configFile<<std::endl;
GyyBot.runBot(configFile);
    return 0;
}
