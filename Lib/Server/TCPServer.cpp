#include "TCPServer.hpp"
#include <iostream>

namespace Servers {

volatile bool keep_running = true;

// Implementação do manipulador de sinal
void signal_handler(int signum) {
    std::cerr << "\nSinal de interrupcao (" << signum << ") recebido. Encerrando..." << std::endl;
    keep_running = false;
}

// Implementação do construtor
TCPServer::TCPServer(int port) : port_(port), server_socket_(INVALID_SOCKET_VALUE), client_socket_(INVALID_SOCKET_VALUE) {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed." << std::endl;
        }
    #endif
}

// Implementação do destrutor
TCPServer::~TCPServer() {
    if (client_socket_ != INVALID_SOCKET_VALUE) {
        #ifdef _WIN32
            closesocket(client_socket_);
        #else
            close(client_socket_);
        #endif
    }
    if (server_socket_ != INVALID_SOCKET_VALUE) {
        #ifdef _WIN32
            closesocket(server_socket_);
        #else
            close(server_socket_);
        #endif
    }
    #ifdef _WIN32
        WSACleanup();
    #endif
}

// Implementação de Start()
bool TCPServer::Start() {
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ == INVALID_SOCKET_VALUE) {
        std::cerr << "Socket creation failed." << std::endl;
        return false;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_socket_, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed." << std::endl;
        return false;
    }

    if (listen(server_socket_, 3) < 0) {
        std::cerr << "Listen failed." << std::endl;
        return false;
    }

    std::cout << "Servidor escutando na porta " << port_ << "..." << std::endl;
    std::cout << "Pressione Ctrl+C para encerrar." << std::endl;
    return true;
}

// Implementação de AcceptClient()
bool TCPServer::AcceptClient() {
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    client_socket_ = accept(server_socket_, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_socket_ == INVALID_SOCKET_VALUE) {
        return false;
    }
    std::cout << "Conexao aceita." << std::endl;
    return true;
}

// Implementação de ReceiveMessage()
std::string TCPServer::ReceiveMessage() {
    if (client_socket_ == INVALID_SOCKET_VALUE) {
        std::cerr << "Client socket is not valid." << std::endl;
        return "";
    }

    int message_size;
    recv(client_socket_, (char*)&message_size, sizeof(message_size), 0);

    std::string received_message(message_size, '\0');
    recv(client_socket_, &received_message[0], message_size, 0);

    return received_message;
}

// Implementação de DisconnectClient()
void TCPServer::DisconnectClient() {
    if (client_socket_ != INVALID_SOCKET_VALUE) {
        #ifdef _WIN32
            closesocket(client_socket_);
        #else
            close(client_socket_);
        #endif
        client_socket_ = INVALID_SOCKET_VALUE;
    }
}

bool CreateServer(int port) {
    TCPServer server(port);
    if (!server.Start()) {
        return false;
    }

    signal(SIGINT, signal_handler);

    while (keep_running) {
        if (server.AcceptClient()) {
            std::string message = server.ReceiveMessage();
            if (!message.empty()) {
                std::cout << "Mensagem recebida: " << message << std::endl;
            }
        }
    }

    server.DisconnectClient();
    std::cout << "Servidor encerrado." << std::endl;
    
    return true;
}

} // namespace Servers