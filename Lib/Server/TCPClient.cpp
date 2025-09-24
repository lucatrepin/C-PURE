#include "TCPClient.hpp"
#include <iostream>

namespace Clients {

// Construtor
TCPClient::TCPClient(const std::string& ip_address, int port) 
    : ip_address_(ip_address), port_(port), client_socket_(INVALID_SOCKET_VALUE) {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed." << std::endl;
            throw std::runtime_error("WSAStartup failed.");
        }
    #endif
}

// Destrutor
TCPClient::~TCPClient() {
    if (client_socket_ != INVALID_SOCKET_VALUE) {
        #ifdef _WIN32
            closesocket(client_socket_);
        #else
            close(client_socket_);
        #endif
    }
    #ifdef _WIN32
        WSACleanup();
    #endif
}

// Implementação de Connect()
bool TCPClient::Connect() {
    client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_ == INVALID_SOCKET_VALUE) {
        std::cerr << "Socket creation failed." << std::endl;
        return false;
    }

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_);
    
    if (inet_pton(AF_INET, ip_address_.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        #ifdef _WIN32
            closesocket(client_socket_);
        #else
            close(client_socket_);
        #endif
        return false;
    }

    if (connect(client_socket_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed. Make sure the server is running." << std::endl;
        #ifdef _WIN32
            closesocket(client_socket_);
        #else
            close(client_socket_);
        #endif
        return false;
    }

    std::cout << "Conectado ao servidor." << std::endl;
    return true;
}

// Implementação de SendMessage()
bool TCPClient::SendMessage(const std::string& message) {
    if (client_socket_ == INVALID_SOCKET_VALUE) {
        std::cerr << "Cannot send message. Socket not connected." << std::endl;
        return false;
    }
    
    int size = message.size();
    send(client_socket_, (const char*)&size, sizeof(size), 0);
    
    return send(client_socket_, message.c_str(), size, 0) != -1;
}

} // namespace Clients