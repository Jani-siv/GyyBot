#include "../include/Json.h"

Json::Json()
    {
    }
Json::~Json()
{
}

void Json::parseScenes(std::string payload, std::map<int,std::string>&scenes)
{
    scenes.clear();
    std::string temp;
    size_t begin, end, delimiter;
    begin = payload.find_first_of("current-scene");
    if (begin < payload.length())
    {
        end = payload.find_first_of(",");
        if (end < payload.length())
        {
            delimiter = payload.find_first_of(":");
            if (delimiter < payload.length())
            {
                temp = payload.substr(begin-1,end-2);
                scenes[0] = temp.substr(delimiter+1, temp.length());
            }
        }
    }
    temp = payload.substr(temp.length(),payload.length());
    payload = temp;
    int i = 1;
    while (payload.length() > 16)
    {
    begin = payload.find_first_of("[{");
    if (begin > payload.length())
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
        }
        found = 0;
        auto it = scenes[i];
        std::cout<<i<<" "<<answer.length()<<" len: "<<it<<std::endl;
        i++;
    }
    temp = payload.substr(end+3,payload.length());
    payload = temp;
    }
}
