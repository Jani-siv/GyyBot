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
    this->joinChannel();
    this->sendPrivMsg("Bot has arrived in stream");
    this->listenBroadCast();
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
    for (int i = 0; i < 7; i++)
        {
            std::cout<<this->connection.getData();
        }
}

void Bot::joinChannel()
{
    std::string channel = this->lowerCase(this->handle.settings.channel);
    std::string payload = "JOIN " + channel;
    this->connection.sendData(payload);
    for (int i=0; i < 3; i++)
    {
        std::cout<<this->connection.getData();
    }
}

void Bot::leaveChannel()
{
    std::string channel = this->lowerCase(this->handle.settings.channel);
    std::string payload = "PART " + channel;
    this->connection.sendData(payload);
}

void Bot::sendPrivMsg(std::string payload)
{
    std::string channel = this->lowerCase(this->handle.settings.channel);
    std::string msg = "PRIVMSG " + channel + " :" + payload;
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

void Bot::listenBroadCast()
{

    std::string msg;
    std::string owner = this->lowerCase(this->handle.settings.channel);
    while(true)
        {
            msg = this->connection.getData();
            if (msg.compare(0,4,"PING") == 0)
            {
                std::cout<<"Connection testing"<<std::endl;
            }
            else if (msg.compare(0,4,"PONG") == 0)
            {
                std::cout<<"connection answer"<<std::endl;
            }
            else
            {
                std::cout<<msg<<std::endl;
                size_t pos = msg.find_last_of('!');
                if (msg.compare(pos,5,"!QUIT") == 0)
                    {
                        pos = msg.find_last_of('#');
                        if (msg.compare(pos,owner.length(),owner) == 0)
                            {
                                break;
                            }
                    }
            }
        }
}
