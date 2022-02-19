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
}

