#ifndef GAMES_HPP
#define GAMES_HPP

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <vector>

#include "Util.cpp"
#include "Characters.hpp"

namespace Games
{

typedef sf::Keyboard::Scan::Scancode Scan;

class Game {
private:
    bool gameOver = false;
    float normalDelta;
    sf::Clock clockDelta;
    sf::Clock clockDelay;
    
    virtual void Process(float delta) {

    }
    virtual void Draw() {

    }
public:
    virtual void Run() {
        while(!gameOver) {
            float delta = clockDelta.restart().asSeconds();
            clockDelay.restart();
            Process(delta);
            Draw();
            float delay = clockDelay.getElapsedTime().asSeconds();
            if (delay < normalDelta) ut::s(normalDelta - delay);
        }
        char tecla = std::cin.get();
        ut::clearCMD();
        std::cout << tecla << std::endl;
        ut::s(10); // remover 
        if (tecla == 'a') Run();
    }
};

class GameASCII : public Game {
private:
    int frame;
public:
    GameASCII(int width = 120, int height = 30, int frames = 60, char nulo = '#', char player = 'P', char obstacle = 'O', char enemie = 'E') : Width(width), Height(height), Frames(frames), Nulo(nulo), Player(player), Obstacle(obstacle), Enemie(enemie)  {
        
    }
    bool GameOver = false;
    int Width;
    int Height;
    int Frames;

    char Nulo;
    char Player;
    char Obstacle;
    char Enemie;

    std::vector<char> window;
    
    void Process(float delta) override = 0;
    void Draw() override {
        if (frame == Frames) {
            frame = 0;
            ut::clearCMD();
        }
        std::string str = "";
        for(int h = 0; h < Height; h++) for(int w = 0; w < Width; w++) str += window[h * Width + w];
        std::cout << str;
    }
};

}

#endif