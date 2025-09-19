#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <csignal>

#include "Server.cpp"

#include <SFML/System/Sleep.hpp>

using namespace Servers;
using namespace sf;

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#endif

// Uma classe para gerenciar o cliente TCP
class TCPClient {
private:
    std::string ip_address_;
    int port_;
    // O tipo do socket e definido pelo SO, por isso a macro
    #ifdef _WIN32
        SOCKET client_socket_;
    #else
        int client_socket_;
    #endif
public:
    // Construtor: inicializa o cliente e a Winsock (se for Windows)
    TCPClient(const std::string& ip_address, int port) : ip_address_(ip_address), port_(port), client_socket_(INVALID_SOCKET) {
        #ifdef _WIN32
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                std::cerr << "WSAStartup failed." << std::endl;
            }
        #endif
    }

    // Destrutor: fecha o socket e limpa os recursos
    ~TCPClient() {
        if (client_socket_ != INVALID_SOCKET) {
            #ifdef _WIN32
                closesocket(client_socket_);
                WSACleanup();
            #else
                close(client_socket_);
            #endif
        }
    }

    // Tenta conectar ao servidor. Retorna true se a conexao for bem-sucedida.
    bool Connect() {
        // 1. Criar o socket
        client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket_ == INVALID_SOCKET) {
            std::cerr << "Socket creation failed." << std::endl;
            return false;
        }

        // Preparar a estrutura de endereco do servidor
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

        // 2. Conectar ao servidor
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
    
    // Envia a mensagem. Retorna true se a mensagem for enviada com sucesso.
    bool SendMessage(const std::string& message) {
        if (client_socket_ == INVALID_SOCKET) {
            std::cerr << "Cannot send message. Socket not connected." << std::endl;
            return false;
        }
        
        // Envia o tamanho da string
        int size = message.size();
        send(client_socket_, (const char*)&size, sizeof(size), 0);
        
        // Envia a string em si
        return send(client_socket_, message.c_str(), size, 0) != -1;
    }
};

// --- Funcao Principal (main) ---
int main() {
    // Agora o codigo no main e muito mais limpo
    TCPClient client("127.0.0.1", 8080);
    
    if (client.Connect()) {
        std::string my_message = "Ola, servidor!";
        client.SendMessage(my_message);
    }
    sf::sleep(sf::seconds(2)); // Apenas para garantir que a mensagem seja enviada antes de fechar o socket
    return 0; // O destrutor da classe sera chamado aqui, fechando o socket automaticamente
}