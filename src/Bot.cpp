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
    this->connection.initSocket(handle.settings.server,handle.settings.port);
    this->authenticate();
    std::cout<<this->connection.getData()<<std::endl;

}

void Bot::authenticate()
{
    std::string payload = "PASS " + this->handle.settings.Oauth;
    this->connection.sendData(payload);
    std::string nick;
    for(int i=0; i < this->handle.settings.username.length(); i++)
    {
        nick += std::tolower(this->handle.settings.username[i]);
    }

    payload = "NICK " + nick;
    this->connection.sendData(payload);
}
