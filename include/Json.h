#pragma once
#include <string>
#include <iostream>
#include <map>
class Json{
    public:
        Json();
        ~Json();
        void parseScenes(std::string payload, std::map<int,std::string>&scenes);
    private:
};

