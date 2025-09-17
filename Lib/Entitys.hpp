#ifndef ENTITYS_HPP
#define ENTITYS_HPP

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Node.hpp"

// entidades simples que servem para ser criadas em massa de forma leve
namespace Entitys
{

template <typename TypeShape>
class Entity : public Node::Node2D {
protected:
    std::unique_ptr<TypeShape> Shape;
public:
    virtual void Move(sf::Vector2f vel) { Shape->move(vel); }
    virtual void Draw(sf::RenderWindow& window) { window.draw(*Shape); }

    void Start() override = 0;
    void Process(float delta) override = 0;
};

class EntityRect : public Entity<sf::RectangleShape> {
public:
    EntityRect(sf::Vector2f size, sf::Vector2f pos) : Entity() {
        Shape = std::make_unique<sf::RectangleShape>(size);
        Shape->setPosition(pos);
    }

    void Start() override = 0;
    void Process(float delta) override = 0;
};

class EntityConvex : public Entity<sf::ConvexShape> {
public:
    EntityConvex(int pointCount, std::vector<sf::Vector2f> points, sf::Vector2f pos) : Entity() {
        Shape = std::make_unique<sf::ConvexShape>(pointCount);
        for(int i = 0; i < pointCount; i++) Shape->setPoint(i, points[i]);
        Shape->setPosition(pos);
    }

    void Start() override = 0;
    void Process(float delta) override = 0;
};

class EntityImg : public Entity<sf::Sprite> {
public:
    EntityImg(sf::Texture& texture, sf::Vector2f pos) : Entity() {
        Shape = std::make_unique<sf::Sprite>(texture);
        Shape->setPosition(pos);
    }

    void Start() override = 0;
    void Process(float delta) override = 0;
};

}

#endif