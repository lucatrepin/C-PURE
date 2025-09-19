#include <vector>
#include <cstdlib>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>

using namespace sf;

using namespace std;

namespace Games
{

class Game {
private:
    bool GameOver = false;
    float Delta;
    float NormalDelta;
    sf::Clock ClockDelay;
    float AverageFPS;
    //bool DrawFPS = true;
    
public:
    sf::RenderWindow window;
    Game(sf::VideoMode videoMode, const string& title, int frames = 2000000000) {
        window.create(videoMode, title);
        SetMaxFrames(frames);
        system("Server.exe 8079");
    }
    virtual void Start() {
        while (!GameOver) {
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
            //if (DrawFPS) std::cout << "FPS: " << AverageFPS << std::endl;   
        }
    }
    virtual void Process(float delta) = 0;
    virtual void Draw() {
        
    }

    void SetMaxFrames(int frames) { NormalDelta = 1000000.f / frames; }
    //void SetDrawFPS(bool state) { DrawFPS = state; }
};

}