#include "../include/Obs.h"
#include <unistd.h>

Obs::Obs()
{

}
Obs::~Obs()
{
}

int Obs::createWebSocket()
{
    int socketFd = this->conn.initSocket(this->connData.address, this->connData.port);
    int ret = 0;
    std::string answer;
    std::string payload = this->connectionCommand(this->connData.address, this->connData.port);
    ret = send(socketFd,payload.c_str(),payload.size(),0);
    if (ret < 0)
    {
        std::cerr<<"Error send to websocket"<<std::endl;
        return -1;
    }
    else
    {
    for (int i = 0; i < 5; i++)
    {
        std::cout<<this->conn.getData(socketFd)<<std::endl;
    }
    //dummy read
    this->conn.readWebSock(socketFd, this->dataStorage);
    return socketFd;
    }
}


int Obs::initConnection(std::string address, int port)
{
    this->connData.address = address;
    this->connData.port = port;
    int socketFd = this->createWebSocket();
    if (socketFd >= 0)
        {
            return socketFd;
        }
    else
    {
        std::cerr<<"Error creating websocket"<<std::endl;
        return -1;
    }

}
void Obs::setScene(std::string scene,int socketFd)
{
    std::string sceneCommand = "{\"request-type\":\"SetCurrentScene\",\"scene-name\":\""+scene+"\",\"message-id\":\"1\"}";

    std::string scene1 = "{\"request-type\":\"SetCurrentScene\",\"scene-name\":\"ubuntu\",\"message-id\":\"1\"}";
    std::string scene2 = "{\"request-type\":\"SetCurrentScene\",\"scene-name\":\"ubuntu2\",\"message-id\":\"1\"}";
    
    //send commands
    if (this->serverVersion == false)
    {
        if (scene.find("sub") == 0) 
        {
            this->conn.sendWebSock(scene2,socketFd);
            //read feed back
            this->conn.readWebSock(socketFd,this->dataStorage);
            sleep(5);
            this->conn.sendWebSock(scene1,socketFd);
            this->conn.readWebSock(socketFd,this->dataStorage);
        }
        else
        {
            this->conn.sendWebSock(sceneCommand,socketFd);
            this->conn.readWebSock(socketFd,this->dataStorage);
        }
    }
    else
    {
        this->conn.serverSendWebSock(sceneCommand,socketFd);
    }
}

void Obs::getScenes(int socketFd)
{
    std::string command = "{\"request-type\":\"GetSceneList\",\"message-id\":\"1\"}";
    int ret = 0;
    if (this->serverVersion == false) 
    {
        ret = this->conn.sendWebSock(command,socketFd);
    }
    else 
    {
        ret = this->conn.serverSendWebSock(command,socketFd);
    }
    std::cout<<"return value on function: "<<ret<<std::endl;
    if (ret < 0)
    {
         std::cerr<<"error in sending packet to websocket"<<std::endl;  
    }
    else
    {
        std::cout<<"Saving data to dataStorage"<<std::endl;
        this->dataStorage.clear();
        if (this->serverVersion == false)
        {
        
        this->conn.readWebSock(socketFd,this->dataStorage);
    
        }
        else
        {
            std::cout<<"Obs answer to server:"<<std::endl;
            std::string payload = this->conn.serverReadData(socketFd);
            std::cout<<payload<<std::endl;
            this->dataStorage.push_back(payload);
        }
    }
}

void Obs::getAvailableRequest(int socketFd)
{
    std::string command = "{\"request-type\":\"GetVersion\",\"message-id\":\"2\"}";
    int ret = this->conn.sendWebSock(command, socketFd);
    if (ret < 0)
    {
        std::cerr<<"error in sending packet to websocket"<<std::endl;
}
}

void Obs::saveReplay(int socketFd, std::string current)
{
    std::string command = "{\"request-type\":\"SaveReplayBuffer\",\"message-id\":\"3\"}";
    int ret = this->conn.sendWebSock(command, socketFd);
    if (ret >= 0)
    {
        sleep(2);
        command = "{\"request-type\":\"SetCurrentScene\",\"scene-name\":\"replay\",\"message-id\":\"4\"}";
        ret = this->conn.sendWebSock(command, socketFd);
        //set dynamic time from obs settings
        //to-do function what get buffer time from obs
        //this need set manually from settings file
        sleep(20);
        command = "{\"request-type\":\"SetCurrentScene\",\"scene-name\":\"" + current + "\",\"message-id\":\"4\"}";
        ret = this->conn.sendWebSock(command, socketFd);
    }
}


std::string Obs::connectionCommand(std::string address, int port)
{
    std::stringstream ss;
    std::string strPort = std::to_string(port);
    std::string get = "GET / HTTP/1.1";
    std::string host ="Host: " + address + ":" + strPort;
    std::string userAgent = "User-Agent:GyyBot/0.1";
    std::string accept ="Accept: */*";  
    std::string upgrade = "Upgrade: websocket";
    std::string connection = "Connection: upgrade";
    std::string SecWebSocketKey = "Sec-WebSocket-key:dGhlIHNhbXBsZSBub25jZQ==";
    std::string origin = "Origin:" + address + ":" + strPort;
    std::string SecWebSocketProtocol = "Sec-WebSocket-Protocol: chat, superchat";
    std::string SecWebSocketVersion = "Sec-WebSocket-Version: 13";
    ss  << get << "\r\n"
        << host << "\r\n" 
        << userAgent << "\r\n"
        << accept << "\r\n"
        << upgrade << "\r\n" 
        << connection << "\r\n" 
        << SecWebSocketKey << "\r\n" 
        << origin << "\r\n" 
        << SecWebSocketProtocol << "\r\n"
        << SecWebSocketVersion << "\r\n\r\n";
    std::string payload = ss.str();
    return payload;
}

std::string Obs::getLastRead()
{
    //this->dataStorage.clear();
    //this->conn.readWebSock(socketFd, this->dataStorage);
    std::cout<<"getting data from storage"<<std::endl;
return this->dataStorage.back();
}
