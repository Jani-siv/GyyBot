#include "../include/FileHandle.h"

FileHandle::FileHandle()
{
    data settingsData;
    this->settings = &settingsData;
}
FileHandle::~FileHandle()
{
}
void FileHandle::getDataFromFile(std::string filename)
{
    this->openFile(filename);
}
int FileHandle::openFile(std::string filename)
{
    std::cout<<"opening file"<<std::endl;
    std::string line;
    std::ifstream inputFile(filename.c_str());
    if (inputFile.is_open())
    {
        while(!inputFile.eof())
        {
            std::getline(inputFile,line);
            this->parseSettingsFile(line);
        }
    }
    else
    {
        std::cerr<<"Cannot open settings file: "<<filename<<"\n";
        exit(-1);
    }
    inputFile.close();
    return 0;
}

void FileHandle::parseSettingsFile(std::string str)
{
if (str.compare(0,6,"OAUTH$") == 0)
{
    std::cout<<"found oauth"<<std::endl;
}
else if (str.compare(0,7,"SERVER$") == 0)
{
    std::cout<<"found server"<<std::endl;
}
else if (str.compare(0,5,"USER$") == 0)
{
    std::cout<<"found user"<<std::endl;
}
else if (str.length() == 0 || str.compare(0,1,"#") == 0)
{}
else
std::cerr<<"Parsing error"<<std::endl;
}
