#include <vector>
#include <cstdlib>
#include <iostream>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>

#include <Server/TCPServer.hpp>
#include <Server/TCPClient.hpp>

using namespace sf;
using namespace std;

namespace Games {

class Game {
private:
    bool GameOver = false;
    float Delta;
    float NormalDelta;
    sf::Clock ClockDelay;
    float AverageFPS;
    bool DrawFPS = false; // Comeca como false
    
    // Inicializa os ponteiros como nullptr para evitar segfaults
    Servers::TCPServer* Server = nullptr;
    Clients::TCPClient* Client = nullptr;

public:
    sf::RenderWindow window;
    
    Game(sf::VideoMode videoMode, const string& title, int frames = 2000000000) {
        window.create(videoMode, title);
        SetMaxFrames(frames);
        SetDrawFPS(true);
    }

    // Destrutor para liberar a memoria alocada
    virtual ~Game() {
        if (Client != nullptr) {
            delete Client;
            Client = nullptr;
        }
        if (Server != nullptr) {
            delete Server;
            Server = nullptr;
        }
    }

    virtual void ProcessEvent(const sf::Event& event) = 0;
    
    virtual void Start() {
        while (!GameOver) {
            // Processa eventos da janela para que ela nao trave
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    GameOver = true;
                } else {
                    ProcessEvent(event);
                }
            }

            ClockDelay.restart();
            Process(Delta / 1000000.f);
            window.clear(sf::Color::Black);
            Draw();
            window.display();
            
            float delay = ClockDelay.getElapsedTime().asMicroseconds();
            if (delay < NormalDelta) {
                sf::sleep(sf::microseconds(NormalDelta - delay));
                Delta = NormalDelta;
                AverageFPS = 1000000.0f / NormalDelta;
            } else {
                Delta = delay;
                AverageFPS = 1000000.0f / delay;
            }
            
            // So tenta enviar a mensagem se o cliente for valido
            if (DrawFPS && Client != nullptr) {
                Client->SendMessage("FPS: " + to_string(AverageFPS));
            }
        }
    }
    
    virtual void Process(float delta) = 0;
    virtual void Draw() {}

    void SetMaxFrames(int frames) { NormalDelta = 1000000.f / frames; }
    
    void SetDrawFPS(bool state) {
        if (state && !DrawFPS) {
            DrawFPS = true;
            Servers::CreateServer(8080);
        } else if (!state && DrawFPS) {
            DrawFPS = false;
            
        }
    }
};

}