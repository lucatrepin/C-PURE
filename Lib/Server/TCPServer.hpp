#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <csignal>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #define SOCKET_TYPE SOCKET
    #define INVALID_SOCKET_VALUE INVALID_SOCKET
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #define SOCKET_TYPE int
    #define INVALID_SOCKET_VALUE -1
#endif

namespace Servers {

extern volatile bool keep_running;

void signal_handler(int signum);

class TCPServer {
public:
    TCPServer();
    TCPServer(int port);
    ~TCPServer();
    
    bool CreateServer(int port);
    bool Start();
    bool AcceptClient();
    std::string ReceiveMessage();
    void DisconnectClient();

private:
    int port_;
    SOCKET_TYPE server_socket_;
    SOCKET_TYPE client_socket_;
};

} // namespace Servers

#endif