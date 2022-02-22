#include "../include/Obs.h"

Obs::Obs()
{
}
Obs::~Obs()
{
}
void Obs::initConnection(std::string address, int port)
{
    //TO-DO create random key
    //TO-DO set userAgent version number ;)
    this->sockFd = this->conn.initSocket(address,port);
    std::string payload = this->connectionCommand(address,port);
    std::cout<<"sending: "<<std::endl;
    std::cout<<payload<<std::endl;
    //HTTP 1.1
    send(this->sockFd,payload.c_str(),payload.size(),0);
    //read answer as plain text
    std::string answer;
    for (int i = 0; i < 5; i++)
    {
    answer = this->conn.getData();
    std::cout<<answer;
    }
    //this on only websock protocol
    //dummy reading
    std::string scene1 = "{\"request-type\":\"SetCurrentScene\",\"scene-name\":\"ubuntu\",\"message-id\":\"1\"}";
    std::string scene2 = "{\"request-type\":\"SetCurrentScene\",\"scene-name\":\"ubuntu2\",\"message-id\":\"1\"}";
    this->conn.readWebSock();
    //send commands
    this->conn.sendWebSock(scene2);
    //read feed back
    this->conn.readWebSock();
    sleep(5);
    this->conn.sendWebSock(scene1);
    this->conn.readWebSock();
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


