#include "Server/SocketListener.h"
#include "Home.h"


int main(int argc ,char **argv) 
{
	int PORT = 80;
	if(argc >1)
	{
		if(strcmp(argv[1],"--help")==0)
		{
			printf("USAGE: socketServer [option] value \r\n");
			printf("\r\nOPTIONS:\r\n");
			printf("\tno option use 80 as default port\r\n");
			printf("\t--help\r\n");
			printf("\t-p value\r\n");
			return 0;
		}
		if(argc ==3)
		{
			if(strcmp(argv[1],"-p")==0)
			{
				PORT=atoi(argv[2]);
				//printf("port is %d",PORT);
			}

		}
	}
	else
	{
		//use default settings
    }
	Home* h = new Home();

    SocketListener listener(SOCKET_TYPE::TCP);
    listener.setHostname("0.0.0.0");
    listener.setPort(PORT);
    listener.setClientHandler(h);
    listener.start();
    return 0;
}
