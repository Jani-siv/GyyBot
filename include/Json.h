#pragma once
#include <string>
#include <iostream>
#include <map>
class Json{
    public:
        Json();
        ~Json();
        void parseString(std::string json, std::map<std::string, std::string>&pairedJson);
    private:
};

