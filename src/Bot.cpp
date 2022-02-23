#include "../include/Bot.h"

Bot::Bot()
{
        this->permission["owner"]=1;
        this->permission["gyybot"]=2;
        this->permission["vip"]=3;
        this->permission["user"]=4;
}
Bot::~Bot()
{
}
void Bot::runBot(std::string settingsFile)
{
    this->handle.getDataFromFile(settingsFile);
    this->handle.openCommandsFile(this->commands);
    this->handle.openUsersFile(this->users);
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
    //set owner
    std::string ownerOfChannel = channel.substr(1,channel.length());
    this->owner = ownerOfChannel;
    this->users[this->owner] = "owner";
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
    while(this->botRunning == true)
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
                this->parseMessages(msg);
                std::cout<<msg<<std::endl;
            }
        }
    this->sendPrivMsg("Bye Bye cruel world");
}

void Bot::parseMessages(std::string message)
{

//:gyyseppi!gyyseppi@gyyseppi.tmi.twitch.tv PRIVMSG #gyyseppi :test
        std::string priv="PRIVMSG";
        size_t pos = message.find(priv);
        std::string command = "!";
        size_t commandPos = message.find_last_of(command);
        if (commandPos > pos && commandPos > 0)
        {
            std::string userCommand;
            std::cout<<"test1"<<std::endl;
            std::string commandMsg = message.substr(commandPos,message.length());
            std::cout<<"find command from str"<<std::endl;
            if (pos > 0)
            {
                size_t comEnd = commandMsg.find(' ');
                if (comEnd > 0)
                {
                    userCommand = commandMsg.substr(0,comEnd);
                }
                else
                {
                    userCommand = commandMsg;
                }
                size_t endPos = message.find_first_of('!');
                std::string username = message.substr(1,endPos-1);
                std::cout<<"user: "<<username<<std::endl;
                for (auto i = this->commands.begin(); i != commands.end(); i++)
                {
                    if (userCommand.find(i->first) < commandMsg.length())
                    {
                        //test user permission
                        std::cout<<"found command"<<std::endl;
                        if(this->checkUserPermission(username,userCommand))
                        {
                            std::cout<<"found command and you got permission"<<std::endl;
                            this->executeCommand(commandMsg,userCommand);
                        }
                        else 
                        {
                            std::cout<<"you don't have permission use that command"<<std::endl;
                        }
                    }
                }
            }
        }
}

void Bot::executeCommand(std::string commandMsg, std::string userCommand)
{
    std::string arg1,arg2;
    //check if there is args
    if (commandMsg.length() > userCommand.length())
    {
        size_t pos = commandMsg.find_first_of(' ');
        std::string allArgs = commandMsg.substr(pos,commandMsg.length());
        pos = allArgs.find_last_of(' ');
        if (pos > allArgs.length())
        {
         //user only one arg commands
            arg1=allArgs;
            std::cout<<"working on it"<<std::endl;
        }
        else 
        {
            //use two arg commands
            arg1=allArgs.substr(0,pos);
            arg2=allArgs.substr(pos,allArgs.length());
            std::cout<<"working on it"<<std::endl;
        }
    }
    //use without args
    else
    {
        std::cout<<userCommand<<std::endl;
        if (userCommand.find("!quit") == 0)
        {
            this->botRunning = false;
        }
        else if (userCommand.find("!scene") == 0)
        {
            this->changeScene();
        }
        else if (userCommand.find("!updateUsers") == 0)
        {
            this->users.clear();
            this->users[this->owner] = "owner";
            this->handle.openUsersFile(this->users);
            std::cout<<"user list and permission updated"<<std::endl;
        }
        else
        {
            std::cout<<"no command yet for: "<<userCommand<<std::endl;
        }
    }
                        

}

bool Bot::checkUserPermission(std::string username, std::string userCommand)
{
    std::string commandPermission,userPermission;
    //max two args
    for (auto i = this->commands.begin(); i != this->commands.end(); i++)
    {
        if (userCommand.find(i->first) <= userCommand.length())
        {
            commandPermission = i->second;
        }
    }
    if (commandPermission.length() <= 0)
    {
        return false;
    }
    for (auto i = this->users.begin(); i != this->users.end(); i++)
    {
        std::cout<<i->first<<" "<<i->second<<std::endl;
        if (username.find(i->first) <= username.length())
        {
            std::cout<<"found permission"<<std::endl;
            userPermission = i->second;
        }
    }
    if (userPermission.length() == 0)
    {
        userPermission = "user";
    }
    int up, cp;
    for (auto i = this->permission.begin(); i != this->permission.end(); i++)
    {
        if (userPermission.find(i->first) <= userPermission.length())
        {
            up = i->second;
        }
        if (commandPermission.find(i->first) <= commandPermission.length())
        {
            cp = i->second;
        }
    }

 if (up <= cp)
 {
     return true;
 }
 else
 {
     return false;
 }

}

void Bot::changeScene()
{
    this->obs.initConnection("192.168.0.224",4444);
}

void initUsers()
{

}
