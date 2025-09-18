#ifndef DINO_HPP
#define DINO_HPP

#include "../Lib/Characters.hpp"
#include "../Lib/Games.hpp"

using namespace Games;

class Dino : public Characters::CharacterAdvanced<sf::RectangleShape> {
public:
    Dino(sf::Vector2f size, sf::Vector2f pos, sf::Vector2f vel) : Characters::CharacterAdvanced<sf::RectangleShape>(size, pos, vel) {}

    void Start() override {
        
    }
    void Process(float delta) override {
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        //     if (state == Enum::PreJump) {
        //         Jump();
        //     } else if (state != Enum::Attacking) {
        //         PreJump();
        //     }
        // }
    }
};

#endif