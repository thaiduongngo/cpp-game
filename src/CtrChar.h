#pragma once
#include <SFML/Graphics.hpp>
#include "Pipes.h"

#ifndef JUPMP_DISTANCE
#define JUPMP_DISTANCE -400.f
#endif

class CtrChar : public sf::Sprite
{
private:
    sf::Image image;
    sf::Texture texture;
    sf::Vector2f originalPosition;
    float velocity = 0.f;

public:
    CtrChar();
    const float getVelocity() const;
    void setOriginalPosition(const sf::Vector2f &originalPosition);
    void moveToOriginalPosition();
    void jump();
    void moveAndFall(const float &gravity, const float &deltaTime);
    const bool collidedWEdge(const float &top, const float &bottom) const;
    const bool collidedWPipe(const PairPipe_t &pipe) const;
    const bool passedPipe(const PairPipe_t &pipe, const float pipeSpeed, const float deltaTime) const;
    void reset();
    ~CtrChar();
};

CtrChar::CtrChar() : image(), texture()
{
    // @todo handle error no file found here
    image.loadFromFile("./res/dragon.png");
    texture.loadFromImage(this->image);
    setTexture(this->texture);
}

const float CtrChar::getVelocity() const
{
    return velocity;
};

void CtrChar::setOriginalPosition(const sf::Vector2f &originalPosition)
{
    this->originalPosition = originalPosition;
}

void CtrChar::jump()
{
    velocity = JUPMP_DISTANCE;
}

void CtrChar::moveAndFall(const float &gravity, const float &deltaTime)
{
    velocity += gravity * deltaTime;
    move(0, velocity * deltaTime);
}

const bool CtrChar::collidedWEdge(const float &top, const float &bottom) const
{
    const sf::Vector2i size = getTextureRect().getSize();
    return getPosition().y < bottom || (getPosition().y + size.y > top);
}

const bool CtrChar::collidedWPipe(const PairPipe_t &pipe) const
{
    return getGlobalBounds().intersects(pipe.first.getGlobalBounds()) ||
           getGlobalBounds().intersects(pipe.second.getGlobalBounds());
}

const bool CtrChar::passedPipe(const PairPipe_t &pipe, const float pipeSpeed, const float deltaTime) const
{
    return pipe.first.getPosition().x + pipe.first.getSize().x < getPosition().x && pipe.first.getPosition().x + pipe.first.getSize().x > getPosition().x + pipeSpeed * deltaTime;
}

void CtrChar::moveToOriginalPosition()
{
    setPosition(originalPosition);
}

void CtrChar::reset()
{
    moveToOriginalPosition();
    velocity = 0.f;
}

CtrChar::~CtrChar()
{
}
