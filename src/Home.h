

#ifndef UNIXSOCKET_HOME_H
#define UNIXSOCKET_HOME_H

#include "Server/ClientHandler.h"
#include <netdb.h>  //gethostbyname
#include <arpa/inet.h>  //ntohl
#include <string.h>
class Home : public ClientHandler {
public:
    void handleRequest(int,char*);
    std::vector<std::string> parseRequest(char*);
    int get_local_ip(char* ip);
    int Int2Ip(uint uiIp, char* strOut);

};


#endif //UNIXSOCKET_HOME_H

