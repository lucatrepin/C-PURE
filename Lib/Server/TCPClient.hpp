#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <string>
#include <stdexcept>

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

namespace Clients {

class TCPClient {
public:
    TCPClient();
    TCPClient(const std::string& ip_address, int port);
    ~TCPClient();

    bool Connect();
    bool SendMessage(const std::string& message);

private:
    std::string ip_address_;
    int port_;
    SOCKET_TYPE client_socket_;
};

} // namespace Clients

#endif