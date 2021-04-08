

#ifndef UNIXSOCKET_SOCKETLISTENER_H
#define UNIXSOCKET_SOCKETLISTENER_H

#include <string>
#include <thread>
#include <sys/socket.h>
#include <sys/un.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "ClientHandler.h"

enum SOCKET_TYPE { UNIX, TCP };

class SocketListener {
private:
    int _serverSocket;
    SOCKET_TYPE _type;

    // unix
    std::string _path;

    // tcp
    std::string _hostname;
    int _port;

    bool _initialized;
    ClientHandler* _clientHandler;
    std::vector<std::thread> _threads;

public:
    SocketListener() = default;
    explicit SocketListener(const SOCKET_TYPE&);
    ~SocketListener();

    // mutators
    void setPath(const std::string&);
    void setHostname(const std::string&);
    void setPort(const int& port);
    void setClientHandler(ClientHandler*);

    // accessors
    const std::string& getPath() const;
    const std::string& getHostname() const;
    int getPort() const;
    const ClientHandler* getClientHandler() const;

    void init();
    void start();
};


#endif //UNIXSOCKET_SOCKETLISTENER_H
