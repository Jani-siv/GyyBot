#include "../include/Obs.h"

Obs::Obs()
{
}
Obs::~Obs()
{
}
void Obs::initConnection(std::string address, int port)
{
    this->sockFd = this->conn.initSocket(address,port);
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
    std::cout<<"sending: "<<std::endl;
    std::cout<<payload<<std::endl;
    send(this->sockFd,payload.c_str(),payload.size(),0);
    //modify here
    std::string answer;
    while(true)
    {
    answer = this->conn.getData();
    std::cout<<answer<<std::endl;
    }
} 
