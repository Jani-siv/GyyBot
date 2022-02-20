#include "../include/Bot.h"

Bot::Bot()
{
}
Bot::~Bot()
{
}
void Bot::runBot(std::string settingsFile)
{
    this->handle.getDataFromFile(settingsFile);
    this->connection.initSocket("127.0.0.1",5555);    
}

