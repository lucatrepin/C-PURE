#include <SFML/Graphics.hpp>

#include <iostream>

#include "../Lib/Games.hpp"

using namespace std;
using namespace Games;
    
class DinoGame : public Games::Game {
private:
    sf::RectangleShape dino;

    void Process(float delta) override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
    void Draw() override {
        window.draw(dino);
    }
    void ProcessEvent(const sf::Event& event) override {
        // Processa outros eventos aqui, se necessario
    }

public:
    DinoGame(sf::VideoMode videoMode, const string& title, int frames = 2000000000) : Game(videoMode, title, frames) {
        dino = sf::RectangleShape(sf::Vector2f(50, 50));
        dino.setFillColor(sf::Color::Green);
        dino.setPosition(100, 100);
    }
};

int main() {
    DinoGame game(sf::VideoMode(800, 600), "Dino Not Net", 60);
    game.Start();
    return 0;
}