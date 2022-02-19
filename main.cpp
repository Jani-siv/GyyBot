#include <iostream>
#include <strings.h>
int main(int argc, char *argv[])
{
if (argc  < 2)
{
    std::cerr<<"Please set config filename"<<"\n";
    exit(-1);
}
    std::cout<<"Hello there.. My name is GyyBot"<<"\n";
    std::cout<<"config filename is: "<<argv[1] <<"\n";
return 0;
}
