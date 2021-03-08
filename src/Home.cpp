
#include "Home.h"

void Home::handleRequest(int clientSock, char* msg) {
    char buff[MAX_BYTES];
    recv(clientSock, &buff, MAX_BYTES, 0);
    std::cout << buff << std::endl;

    // parse the request
    std::vector<std::string> requestLines = this->parseRequest(buff);

    char resp[4096] ="";
    char ip[128]="";
    get_local_ip(ip);
    sprintf(resp, "HTTP/1.1 200 OK\nContent-Type: text/html \n\n %s %s)",msg,ip);
    // return the response
    send(clientSock, resp, strlen(resp), 0);

    // close the client socket
    close(clientSock);
}

std::vector<std::string> Home::parseRequest(char *) {
    return std::vector<std::string>();
}

int Home::get_local_ip(char* ip)
{
    char hostname[128];
    int ret = gethostname(hostname, sizeof(hostname));
    if (ret == -1){
        return -1;
    }
    struct hostent *hent;
    hent = gethostbyname(hostname);
    if (NULL == hent) {
        return -1;
    }
    //get h_addr_list  first h_addr
   // memmove(ip,((struct in_addr*)hent->h_addr)->s_addr,4);
   //
   int ipaddr=((struct in_addr*)hent->h_addr)->s_addr;
    Int2Ip(ipaddr,ip);
    //int i;
    //for(i=0; hent->h_addr_list[i]; i++) {
    //    uint32_t u = ntohl(((struct in_addr*)hent->h_addr_list[i])->s_addr);
    //    std::cout << u << std::endl;
    //}
    return 0;
}

int Home::Int2Ip(uint uiIp, char* strOut)
{
	unsigned char bN1, bN2, bN3, bN4;
	char arrIp[32] = { 0 };
	bN1 = (uiIp)& 0xFF;
	bN2 = (uiIp >> 8) & 0xFF;
	bN3 = (uiIp >> 16) & 0xFF;
	bN4 = (uiIp >> 24) & 0xFF;
	sprintf(strOut, "%d.%d.%d.%d", bN1, bN2, bN3, bN4);
	return 0;
}

