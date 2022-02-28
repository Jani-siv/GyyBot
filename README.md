#GyyBot 0.1 for twitch
#DO NOT USE THIS IF YOU DON'T KNOW WHAT ARE YOU DOING
#THIS IS BUILD ONLY FOR DEMONSTRATION PURPOSE
##############################################################

Bot connect obs and twitch together using obswebsocket 4.9.1
This version run under linux and it's tested with virtual Ubuntu

With this bot it's possible with chat commands in twitch change scenes,
get instant 20 s replay, add users and set permission for users

this version not use authentication between program and obswebsocket and
it will need module to create random connection-id and websocket messages
now those two are static values and not provide secure connection.

at this moment connection address is static for obs host. This won't work with any other
addresses. In obs websocket need be to set listen port 4444 and host address 192.168.0.224

This version is thread's free

For make your own bot you need get new account for bot and get Oauth from twitch to bot account
in settings.txt you need set in line OAUTH$oauth:(bot oauth)

Commands in twitch chat:
!commands # give all avaible commands
!getScenes # return all scenes in chat
!addUser username permission #add new user for bot
!updateUsers #update user permission for bot
!replay # change to last scene in obs list and after 20 second return back to scene where command was set
!scene scenename # change scene to scenename

commands.txt include all chat commands
!command$priviledge

users.txt
username$priviledge

settings.txt
all settings for bot

compiling
This program will need cmake to compile
in download directory
$ cmake -S . -B ./buildDirectory/

usage:
./buildDirectory/Gyybot settings.txt

