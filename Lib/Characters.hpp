#ifndef CHARACTERS_HPP
#define CHARACTERS_HPP

#include <chrono>
#include "Entitys.hpp"

using namespace Entitys;

namespace Characters
{

template <typename TypeShape>
class Character : public Entitys::Entity<TypeShape> {
protected:
    sf::Vector2f Vel;
    sf::Vector2f Direc;

    Character(sf::Vector2f vel, sf::Vector2f direc = {1, 0}) : Entitys::Entity<TypeShape>(), Vel(vel), Direc(direc) {}
    virtual void Move() {
        float x = 0;
        float y = 0;
        if (Direc.x > 0) x = Vel.x * Direc.x;
        else if (Direc.x < 0) x = - Vel.x * Direc.x;
        if (Direc.y > 0) y = - Vel.y * Direc.y;
        else if (Direc.y < 0) y = Vel.y * Direc.y;
        Shape->move({x, y});
    }

    void Start() override = 0;
    void Process(float delta) override = 0;
};

template <typename TypeShape>
class CharacterAdvanced : public Entitys::Entity<TypeShape> {
private:
    sf::Vector2f JumpForce;
    float TimeJump;
    float MinChargeJumpTime;
    float MaxChargeJumpTime;

public:
    CharacterAdvanced(sf::Vector2f size, sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f direc = {1, 0}, sf::Vector2f jumpForce = {0, -10}, float minDurationJump, float maxDurationJump) : Character<TypeShape>(vel, direc), JumpForce(jumpForce), MinChargeJumpTime(minDurationJump), MaxChargeJumpTime(maxDurationJump) {
        Shape = std::make_unique<TypeShape>(size);
        Shape->setPosition(pos);
    }

    virtual void PreJump() {
        if (IsOnFloor()) TimeJump = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now());
    }
    virtual void Jump(float delta) {
        float charge = std::min(MaxChargeJumpTime, std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now()) - TimeJump);
        if (charge > MinChargeJumpTime) Shape->move(JumpForce * charge);
        // usar charge / terminar funcao
    }

    void Start() override = 0;
    void Process(float delta) override = 0;
};

// class CharacterRect : public Character<sf::RectangleShape> {
// public:
//     CharacterRect(sf::Vector2f size, sf::Vector2f pos, sf::Vector2f vel) : Character<sf::RectangleShape>(vel) {
//         Shape = std::make_unique<sf::RectangleShape>(size);
//         Shape->setPosition(pos);
//     }

// };

}

#endif