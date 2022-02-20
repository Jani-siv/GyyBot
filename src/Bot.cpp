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
    this->joinChannel();
    std::cout<<this->connection.getData()<<std::endl;
    this->sendPrivMsg("hello world");
    std::cout<<this->connection.getData()<<std::endl;
    this->leaveChannel();
    std::cout<<this->connection.getData()<<std::endl;
}

void Bot::authenticate()
{
    std::string payload = "PASS " + this->handle.settings.Oauth;
    this->connection.sendData(payload);
    std::string nick = this->lowerCase(this->handle.settings.username);
    payload = "NICK " + nick;
    this->connection.sendData(payload);
}

void Bot::joinChannel()
{
    std::string channel = this->lowerCase(this->handle.settings.channel);
    std::string payload = "JOIN " + channel;
    this->connection.sendData(payload);
}

void Bot::leaveChannel()
{
    std::string channel = this->lowerCase(this->handle.settings.channel);
    std::string payload = "PART " + channel;
    this->connection.sendData(payload);
}

void sendPrivMsg(std::string payload)
{
    std::string channel = this->lowerCase(this->handle.settings.channel);
    std::string msg = "PRIMSG " + channel + " :" + payload;
    this->connection.sendData(msg);
}

std::string Bot::lowerCase(std::string data)
{
    std::string payload;
    for (int i = 0; i < data.length(); i++)
    {
        payload += std::tolower(data[i]);
    }
    return payload;
}
