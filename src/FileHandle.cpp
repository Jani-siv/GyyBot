#include "../include/FileHandle.h"

FileHandle::FileHandle()
{
}
FileHandle::~FileHandle()
{
}
void FileHandle::getDataFromFile(std::string filename)
{
    this->openSettingsFile(filename);
}
int FileHandle::openSettingsFile(std::string filename)
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
        //test data is bigger than 0
        if (    this->settings.server.length() == 0 ||
                this->settings.Oauth.length() == 0 ||
                this->settings.username.length() == 0 ||
                this->settings.channel.length() == 0)
        {
            std::cerr<<"Settings data error check server, Oauth, username and channel from: "<<filename<<std::endl;
            exit(-1);
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
    this->settings.Oauth = this->returnAfterChr(str,'$');
}
else if (str.compare(0,7,"SERVER$") == 0)
{
    std::string server, port;
    int tempPort;
    size_t pos;
    server = this->returnAfterChr(str,'$');
    pos = server.find_last_of(':');
    port = server.substr(pos+1,server.length());
    
    tempPort = std::stoi(port);
    if (tempPort <= std::numeric_limits<short>::max())
    {
    this->settings.port = tempPort;
    this->settings.server = server.substr(0,pos);
    }
    else
    {
        std::cerr<<"Port number is too big"<<std::endl;
        exit(-1);
    }
}
else if (str.compare(0,5,"USER$") == 0)
{
    this->settings.username = this->returnAfterChr(str,'$');
}

else if (str.compare(0,8,"CHANNEL$") == 0)
{
    this->settings.channel = this->returnAfterChr(str,'$');
}
else if (str.compare(0,12,"COMMANDFILE$") == 0)
{
    this->settings.commandfile = this->returnAfterChr(str,'$');
    this->testFilesFromSettings(this->settings.commandfile);
}
else if (str.compare(0,9,"USERFILE$") == 0)
{
    std::cout<<str<<std::endl;
    this->settings.userfile = this->returnAfterChr(str,'$');
    this->testFilesFromSettings(this->settings.userfile);
}
else if (str.compare(0,5,"AUTH$") == 0)
{
    this->settings.authfile = this->returnAfterChr(str,'$');
    this->testFilesFromSettings(this->settings.authfile);
}

else if (str.length() == 0 || str.compare(0,1,"#") == 0)
{}
else
std::cerr<<"Parsing error"<<std::endl;
}
std::string FileHandle::returnAfterChr(std::string str, char a)
{
size_t pos = str.find_first_of(a);
return str.substr(pos+1,str.length());
}


void FileHandle::openUsersFile(std::map<std::string,std::string>&usersmap)
{
    std::ifstream inputFile(this->settings.userfile.c_str());
    if (!inputFile.is_open())
    {
        std::cerr<<"error opening userfile: "<<this->settings.userfile<<std::endl;
        exit(-1);
    }
    std::string line, a,b;
    std::cout<<"reading user file"<<std::endl;
    while(!inputFile.eof())
    {
        std::getline(inputFile,line);
        if (line.length() > 0)
        {
            if (line.compare(0,1,"#") != 0)
            {
                size_t pos = line.find('$');
                a = line.substr(0,pos);
                b = line.substr(pos,line.length());
                usersmap[a]=b;
            }
        }
    }
}


void FileHandle::openCommandsFile(std::map<std::string,std::string>&commands)
{
    std::ifstream inputFile(this->settings.commandfile.c_str());
    if (!inputFile.is_open())
        {
        std::cerr<<"Cannot open command file: "<<this->settings.commandfile<<std::endl;
        exit(-1);
        }
    std::string line, a,b;
    std::cout<<"Reading commandfile"<<std::endl;
    while(!inputFile.eof())
    {
        std::getline(inputFile,line);
        if (line.length() > 0)
        {
            if (line.compare(0,1,"#") != 0)
                {
                size_t pos = line.find('$');
                a = line.substr(0,pos);
                b = line.substr(pos,line.length());
                commands[a] = b;
                }
        }   

    }
}
void FileHandle::testFilesFromSettings(std::string filename)
{
    std::ifstream test(filename.c_str());
    if (!test.is_open())
    {
        std::ofstream test(filename.c_str());
        std::cout<<"Created file: "<<filename<<std::endl;
        test.close();
    }
    else
    {
        test.close();
    }

}
