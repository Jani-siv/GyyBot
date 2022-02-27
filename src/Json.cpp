#include "../include/Json.h"

Json::Json()
    {
    }
Json::~Json()
{
}

void Json::parseScenes(std::string payload, std::map<int,std::string>&scenes)
{
    std::cout<<payload<<std::endl;
    scenes.clear();
    std::string temp;
    size_t begin, end, delimiter;
    begin = payload.find("current-scene");
    if (begin < payload.length())
    {
        end = payload.find("message-id");     
        end -= 3;
        size_t len = end - (begin+16);
        temp = payload.substr(begin+16,len);
        std::cout<<"end: "<<end<<std::endl;
        scenes[0] = temp;
/*
        if (end < payload.length())
        {
            std::cout<<"temp:"<<temp<<std::endl;
            delimiter = temp.find("mes");
            std::cout<<"delimiter: "<<delimiter<<std::endl;
            if (delimiter < payload.length())
            {
                std::cout<<"temp: "<<temp<<std::endl;
                std::string sub = temp.substr(0,temp.length());
                scenes[0] = sub;
                std::cout<<"scene: "<<sub<<std::endl;
            }
        }
  */  
    temp = payload.substr(temp.length(),payload.length());
    payload = temp;
    int i = 1;
    while (payload.length() > 20)
    {
        begin = payload.find_first_of("[{");
        if (begin >= payload.length())
        {
            break;
        }
        end = payload.find_first_of("}]}");
        temp = payload.substr(begin,end);
        size_t endName = temp.find_first_of("sour");
        delimiter = temp.find_first_of(":");
    
         std::string answer = temp.substr(delimiter+2,endName);
        if (answer.compare(0,1,",") != 0 )
        {
            size_t last = answer.find('"');
            int found = 0;
            std::string comp = answer.substr(0,last);
        for (auto j = scenes.begin(); j != scenes.end(); j++)
        {
            if (j->second.compare(comp) == 0 )
            { 
                found = 1;
                break;
            }
        }
        if (found == 0)
        {
        scenes[i] = answer.substr(0,last);
        i++;
        }
        found = 0;
        
    }
    temp = payload.substr(end+3,payload.length());
    payload = temp;
    }
    }
    else
    {
        std::cerr<<"Error wrong data in parser"<<std::endl;
    }
}
