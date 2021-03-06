#include "../include/Bot.h"
#include <asm-generic/socket.h>
#include <iterator>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

Bot::Bot()
{
        this->permission["owner"]=1;
        this->permission["gyybot"]=2;
        this->permission["vip"]=3;
        this->permission["user"]=4;
}
Bot::~Bot()
{
    close(this->twitchSocketFd);
    close(this->obsSocketFd);
}

void Bot::runBotServer(std::string settingsFile, int clientFd)
{
    this->obs.serverVersion = true;
    this->handle.getDataFromFile(settingsFile);
    this->handle.openCommandsFile(this->commands);
    this->handle.openUsersFile(this->users);
        this->botRunning = true;
    this->twitchSocketFd = this->connection.initSocket(handle.settings.server, handle.settings.port);
    this->authenticate();
    this->joinChannel();
    this->sendPrivMsg("Gyy bot is ready");
    this->obsSocketFd = clientFd;
    this->listenBroadCast();
    this->leaveChannel();
    close(this->obsSocketFd);
    close(this->twitchSocketFd);
}

void Bot::runBot(std::string settingsFile)
{
    this->handle.getDataFromFile(settingsFile);
    this->handle.openCommandsFile(this->commands);
    this->handle.openUsersFile(this->users);
    this->twitchSocketFd = this->connection.initSocket(handle.settings.server,handle.settings.port);
    std::cout<<"twitch ok"<<std::endl;
    this->authenticate();
    this->joinChannel();
    this->sendPrivMsg("Bot has arrived in stream");
    this->obsSocketFd = this->obs.initConnection("192.168.0.224",4444);
    this->updateScenes();
    this->listenBroadCast();
    this->leaveChannel();
    std::cout<<this->connection.getData(this->twitchSocketFd)<<std::endl;
}

void Bot::authenticate()
{
    bool con = false;
    int waitingtime = 1;
    
    std::string payload = "PASS " + this->handle.settings.Oauth;
    int ret = 0;
    while (con == false)
    {
    ret = this->connection.sendData(payload,this->twitchSocketFd);
    if (ret > 0)
    {
        con = true;
        break;
    }
    else
    {
        sleep(waitingtime);
        waitingtime = std::pow(waitingtime,2);
    }
    }
    std::string nick = this->lowerCase(this->handle.settings.username);
    payload = "NICK " + nick;
    this->connection.sendData(payload,this->twitchSocketFd);
    for (int i = 0; i < 7; i++)
        {
            std::cout<<this->connection.getData(this->twitchSocketFd);
        }
}

void Bot::joinChannel()
{
    std::string channel = this->lowerCase(this->handle.settings.channel);
    std::string payload = "JOIN " + channel;
    this->connection.sendData(payload,this->twitchSocketFd);
    for (int i=0; i < 3; i++)
    {
        std::cout<<this->connection.getData(this->twitchSocketFd);
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
    this->connection.sendData(payload,this->twitchSocketFd);
}

void Bot::sendPrivMsg(std::string payload)
{
    std::string channel = this->lowerCase(this->handle.settings.channel);
    std::string msg = "PRIVMSG " + channel + " :" + payload;
    this->connection.sendData(msg,this->twitchSocketFd);
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
    int error = 0;
    socklen_t len = sizeof(error);

    while(this->botRunning == true)
        {
            int retval = getsockopt(this->obsSocketFd, SOL_SOCKET,SO_ERROR, &error, &len);
            if (error != 0)
            {
                this->botRunning = false;
                break;
            }
            msg = this->connection.getData(this->twitchSocketFd);
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
    sleep(1);
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
            std::string commandMsg = message.substr(commandPos,message.length());
            std::cout<<"find command from str"<<commandMsg<<std::endl;
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
                    if (userCommand.find(i->first) <= userCommand.length())
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
    std::cout<<"commandmsg: "<<commandMsg<<" usermsg: "<<userCommand<<std::endl;
    if (commandMsg.length() > userCommand.length())
    {
        size_t pos = commandMsg.find_first_of(' ');
        std::string allArgs = commandMsg.substr(pos+1,(commandMsg.length()-1));
        std::cout<<"ARGS: "<<allArgs<<std::endl;
        pos = allArgs.find_last_of(' ');
        if (pos >= allArgs.length())
        {
            std::cout<<"changing scene"<<std::endl;
         //user only one arg commands
            arg1=allArgs.substr(0,allArgs.length()-1);
            if (userCommand.find("!scene") == 0)
            {
                this->changeScene(arg1);
            }
        }
        else 
        {
            //use two arg commands
            arg1=allArgs.substr(0,pos);
            arg2=allArgs.substr(pos+1,allArgs.length()-1);
            if (userCommand.find("!addUser") == 0)
            {
                this->addUser(arg1,arg2);
            }
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
        else if (userCommand.find("!updateUsers") == 0)
        {
            this->users.clear();
            this->users[this->owner] = "owner";
            this->handle.openUsersFile(this->users);
            std::cout<<"user list and permission updated"<<std::endl;
        }
        else if (userCommand.find("!commands") == 0)
        {
            this->showCommands();
        }
        else if (userCommand.find("!getScenes") == 0)
        {
            this->sendPrivMsg(this->showScenes());
        }
        else if (userCommand.find("!getRequest") == 0)
        {
            this->availableRequest();
        }
        else if (userCommand.find("!replay") == 0)
        {
            this->instantReplay();
        }
        else if (userCommand.find("!gamma") == 0)
        {
            this->lastScene();
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
      //  std::cout<<i->first<<" "<<i->second<<std::endl;
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

void Bot::changeScene(std::string scene)
{
   this->obs.setScene(scene,this->obsSocketFd);
}

void initUsers()
{

}

void Bot::addUser(std::string username, std::string permission)
{
    //check permission and user before adding
    std::cout<<"username: "<<username<<" permission:"<<permission<<std::endl;
    std::string payload =  username + "$" + permission;
    this->handle.addUser(payload);
}

void Bot::showCommands()
{
    std::string payload = "Commands: ";
    for (auto i = this->commands.begin(); i != this->commands.end(); i++)
    {
     payload += i->first + " ";   
    }
    this->sendPrivMsg(payload);
    
    
}

void Bot::getScenes(int socketFd)
{
    this->obs.getScenes(socketFd);
}

void Bot::instantReplay()
{
    this->updateScenes();
    auto i = this->scenes[0];
    std::cout<<"current scene:"<<i<<std::endl;
this->obs.saveReplay(this->obsSocketFd,i);
}

void Bot::availableRequest()
{
this->obs.getAvailableRequest(this->obsSocketFd);
}

void Bot::updateScenes()
{
std::cout<<"get last read"<<std::endl;
//int socket = this->obs.initConnection("192.168.0.224",4444);
this->getScenes(this->obsSocketFd);
std::string payload = this->obs.getLastRead();
//close(socket);
std::cout<<"payload"<<std::endl;
std::cout<<payload<<std::endl;
sleep(5);
std::cout<<"start parsing"<<std::endl;
this->parseScenes(payload,this->scenes);
std::cout<<"parsing done"<<std::endl;
//work in this point
}

std::string Bot::showScenes()
{
    std::cout<<"update scenes"<<std::endl;
    this->updateScenes();
    //this->getScenes();
    std::string payload;
    for (auto i = this->scenes.begin(); i != this->scenes.end(); i++)
    {
        payload += std::to_string(i->first);
            payload += ":";
        payload += i->second;
        payload += " ";
    }
    return payload;
}

void Bot::lastScene()
{
    int socket;
    if (this->serverVersion == false)
    {
    socket = this->obs.initConnection("192.168.0.224",4444);
    }
    else
    {
        socket = this->obsSocketFd;
    }
    if (this->gamma == false)
    {
        std::cout<<"change Scene"<<std::endl; 
        this->updateScenes();
        this->beforeGamma = this->scenes[0];
        this->obs.setScene(std::prev(this->scenes.end())->second,socket);
        this->gamma = true;
    }
    
    else if (this->gamma == true)
    {
        std::cout<<"gamma true!!!"<<std::endl;
       // this->updateScenes();
        std::cout<<"scenes updated!!!"<<std::endl;
        this->obs.setScene(this->beforeGamma, socket);
        std::cout<<"scene changet back"<<std::endl;
        this->gamma = false;
    }
}
