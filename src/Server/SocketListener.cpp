

#include <netinet/in.h>
#include <arpa/inet.h>
#include "SocketListener.h"
#include "ClientHandler.h"

SocketListener::SocketListener(const SOCKET_TYPE& type)
    : _type(type),
      _path(""),
      _hostname(""),
      _port(-1),
      _initialized(false) {
}

SocketListener::~SocketListener() {
    for (auto &t: this->_threads) {
        t.join();
    }

    shutdown(this->_serverSocket, 0);
    close(this->_serverSocket);
}

void SocketListener::init() {
    // determine the type of socket
    sockaddr* socketAddr;
    switch (this->_type) {
        case SOCKET_TYPE::UNIX: {
            if (this->_path.empty()) {
                std::cout << "No path provided for socket" << std::endl;
                return;
            }

            sockaddr_un unixSocket{};
            unixSocket.sun_family = AF_UNIX;
            strcpy(unixSocket.sun_path, this->_path.c_str());

            this->_serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
            if (this->_serverSocket < 0) {
                perror("Error creating socket");
                return;
            }
            const int one = 1;
            setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

            unlink(this->_path.c_str());
            if (bind(this->_serverSocket, (struct sockaddr*) &unixSocket, sizeof(unixSocket)) < 0) {
                perror("Failed to connect socket");
                return;
            }

            break;
        }

        case SOCKET_TYPE::TCP: {
            if (this->_hostname.empty()) {
                std::cout << "No hostname provided for socket" << std::endl;
                return;
            }

            if (this->_port == -1) {
                std::cout << "No port provided for socket" << std::endl;
                return;
            }

            sockaddr_in tcpSocket{};
            tcpSocket.sin_family = AF_INET;
            tcpSocket.sin_addr.s_addr = inet_addr(this->_hostname.c_str());
            tcpSocket.sin_port = htons(this->_port);

            this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (this->_serverSocket < 0) {
                perror("Error creating socket");
                return;
            }
            const int one = 1;
            setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
            int bound = bind(this->_serverSocket, (struct sockaddr*) &tcpSocket, sizeof(tcpSocket));
            if (bound < 0) {
                perror("Failed to connect socket");
                return;
            }

            break;
        }

        default: {
            std::cout << "Invalid socket type" << std::endl;
            return;
        }
    }

    this->_initialized = true;
}

void SocketListener::start() {
    char msg[4096]="";
    if (!this->_initialized) {
        this->init();
        if (!this->_initialized) {
            std::cout << "Failed to initialize socket\n";
            return;
        }
    }

    if (listen(this->_serverSocket, 100) < 0) {
        perror("Failed to start listening on socket");
        return;
    }

    switch (this->_type) {
        case SOCKET_TYPE::TCP: {
            std::cout << "Listening on " << this->_hostname << ":" << this->_port << std::endl;
            break;
        }

        case SOCKET_TYPE::UNIX: {
            std::cout << "Listening on " << this->_path << std::endl;
            break;
        }
    }

    int socketErrors = 0;
    while (true) {
        sockaddr_un clientAddr{};
        memset(&clientAddr, 0, sizeof(sockaddr_un));
        unsigned int clientAddrSize = sizeof(clientAddr);
        int clientSock = accept(this->_serverSocket, (sockaddr*) &clientAddr, &clientAddrSize);
        if (clientSock < 0) {
            socketErrors++;
            if (socketErrors > 15) {
                break;
            }

            perror("Failed to accept client");
            continue;
        }

        switch (this->_type) {
            case SOCKET_TYPE::TCP: {
                char clientHostname[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(((struct sockaddr_in*) &clientAddr)->sin_addr.s_addr), clientHostname, INET_ADDRSTRLEN);
                std::cout << "Accepted client connection " << clientHostname << ":" << ((struct sockaddr_in*) &clientAddr)->sin_port << std::endl;
                //std::cout << "<html><body> from :" << clientHostname << ":" << ((struct sockaddr_in*) &clientAddr)->sin_port <<"</body></html>"<< std::endl;
                sprintf(msg,"<html><body> <p><b>Hello Demo</b></p><p>From :%s  </p></body></html>" ,clientHostname );
                break;
            }

            case SOCKET_TYPE::UNIX: {
                std::cout << "Accepted client connection " << ((struct sockaddr_un*) &clientAddr)->sun_path << std::endl;
                break;
            }
        }

        // create thread
        this->_threads.emplace_back(&ClientHandler::handleRequest, this->_clientHandler, clientSock,msg);
    }
}

void SocketListener::setPath(const std::string& path) {
    this->_path = path;
}

void SocketListener::setHostname(const std::string& hostname) {
    this->_hostname = hostname;
}

void SocketListener::setPort(const int &port) {
    this->_port = port;
}

const std::string& SocketListener::getPath() const {
    return this->_path;
}

const std::string& SocketListener::getHostname() const {
    return this->_hostname;
}

int SocketListener::getPort() const {
    return this->_port;
}

void SocketListener::setClientHandler(ClientHandler * clientHandler) {
    this->_clientHandler = clientHandler;
}

const ClientHandler* SocketListener::getClientHandler() const {
    return this->_clientHandler;
}
