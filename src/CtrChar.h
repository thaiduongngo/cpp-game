#pragma once
#include <SFML/Graphics.hpp>
#include "Pipes.h"

constexpr auto JUPMP_DISTANCE = -400.f;
constexpr auto CHAR_IMAGE = "./res/dragon.png";

#ifndef CHAR_ORG_POSITION
#define CHAR_ORG_POSITION sf::Vector2f(100.f, 300.f)
#endif

class CtrChar : public sf::Sprite
{
private:
    sf::Texture texture_;
    float velocity = 0.f;

public:
    CtrChar();
    const float getVelocity() const;
    void moveToOriginalPosition();
    void jump();
    void moveAndFall(const float &gravity, const float &deltaTime);
    const bool collidedWEdge(const float &top, const float &bottom) const;
    const bool collidedWPipe(const PairPipe_t &pipe) const;
    const bool passedPipe(const PairPipe_t &pipe, const float pipeSpeed, const float deltaTime) const;
    void reset();
    ~CtrChar();
};

CtrChar::CtrChar() : texture_()
{
    // @todo handle error no file found here
    texture_.loadFromFile(CHAR_IMAGE);
    texture_.setSmooth(true);
    setTexture(texture_);
}

const float CtrChar::getVelocity() const
{
    return velocity;
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
    setPosition(CHAR_ORG_POSITION);
}

void CtrChar::reset()
{
    moveToOriginalPosition();
    velocity = 0.f;
}

CtrChar::~CtrChar() {}
