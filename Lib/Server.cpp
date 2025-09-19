#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <csignal>

namespace Servers
{

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

// Flag para controle de interrupcao com Ctrl+C
volatile bool keep_running = true;

// Manipulador de sinal
void signal_handler(int signum) {
    std::cerr << "\nSinal de interrupcao (" << signum << ") recebido. Encerrando..." << std::endl;
    keep_running = false;
}

class TCPServer {
private:
    int port_;
    SOCKET_TYPE server_socket_;
    SOCKET_TYPE client_socket_;

public:
    // Construtor: Inicializa o servidor na porta especificada
    TCPServer(int port) : port_(port), server_socket_(INVALID_SOCKET_VALUE), client_socket_(INVALID_SOCKET_VALUE) {
        #ifdef _WIN32
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                std::cerr << "WSAStartup failed." << std::endl;
                // Em um cenario real, voce lancaria uma excecao aqui
            }
        #endif
    }

    // Destrutor: Garante que os sockets sejam fechados corretamente
    ~TCPServer() {
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

    // Inicia o servidor (bind e listen)
    bool Start() {
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

    // Aceita uma nova conexao. Funcao bloqueante.
    bool AcceptClient() {
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        client_socket_ = accept(server_socket_, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket_ == INVALID_SOCKET_VALUE) {
            return false;
        }
        std::cout << "Conexao aceita." << std::endl;
        return true;
    }

    // Recebe uma mensagem do cliente
    std::string ReceiveMessage() {
        if (client_socket_ == INVALID_SOCKET_VALUE) {
            std::cerr << "Client socket is not valid." << std::endl;
            return "";
        }

        int message_size;
        // Recebe o tamanho da mensagem primeiro
        recv(client_socket_, (char*)&message_size, sizeof(message_size), 0);

        std::string received_message(message_size, '\0');
        // Recebe a mensagem com o tamanho esperado
        recv(client_socket_, &received_message[0], message_size, 0);

        return received_message;
    }

    // Desconecta o cliente atual
    void DisconnectClient() {
        if (client_socket_ != INVALID_SOCKET_VALUE) {
            #ifdef _WIN32
                closesocket(client_socket_);
            #else
                close(client_socket_);
            #endif
            client_socket_ = INVALID_SOCKET_VALUE;
        }
    }
};

// --- Funcao Principal (main) ---
int main(int argc, char* argv[]) {
    signal(SIGINT, signal_handler);

    int port = 8080;
    if (argc > 1) {
        port = std::atoi(argv[1]);
    }

    TCPServer server(port);
    if (!server.Start()) {
        return 1;
    }

    while (keep_running) {
        if (server.AcceptClient()) {
            std::string message = server.ReceiveMessage();
            std::cout << "Mensagem recebida: " << message << std::endl;
            server.DisconnectClient();
        }
    }
    
    std::cout << "Servidor encerrado." << std::endl;
    return 0;
}

}
