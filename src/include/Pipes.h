#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Pipe.h"

constexpr auto PIPE_MAX_WIDTH = 110.f;
constexpr auto PIPE_MAX_HEIGHT = 660.f;
constexpr auto PIPE_GAP = 270.f;
constexpr auto PIPE_SPEED = -220.f;
constexpr auto PIPE_IMAGE = "./res/stripes.png";

#ifndef PIPE_COLOR
#define PIPE_COLOR sf::Color::Yellow
#endif

#ifndef PIPE_OUTLINE_COLOR
#define PIPE_OUTLINE_COLOR sf::Color::Red
#endif

typedef Pipe Pipe_t;
typedef std::pair<Pipe_t, Pipe_t> PairPipe_t;
typedef std::vector<PairPipe_t> Pipes_t;

class Pipes
{
private:
    Pipes_t pipes_;
    sf::Texture texture_;

public:
    Pipes();
    void spawnPipe(const float &start, const int &range);
    void movePipes(const float &deltaTime);
    const Pipes_t &getPipes() const;
    const float getPipeSpeed() const;
    const bool offScreen(const size_t &i) const;
    void eraseOffScreenPipe(const size_t &i);
    void reset();
    ~Pipes();
};

Pipes::Pipes() : pipes_(), texture_()
{
}

void Pipes::spawnPipe(const float &start, const int &range)
{
    texture_.loadFromFile(PIPE_IMAGE);
    texture_.setSmooth(true);
    texture_.setRepeated(true);
    const auto vecPipe = sf::Vector2f(PIPE_MAX_WIDTH, PIPE_MAX_HEIGHT);
    const float yPosition = static_cast<float>(abs(std::rand()) % range); // Randomize pipe position
    auto pipeUp = Pipe_t(vecPipe, texture_);
    auto pipeDown = Pipe_t(vecPipe, texture_);
    pipeUp.setPosition(start, yPosition - PIPE_MAX_HEIGHT);
    pipeDown.setPosition(start, yPosition + PIPE_GAP);
    pipes_.push_back(std::move(PairPipe_t(pipeUp, pipeDown)));
}

void Pipes::movePipes(const float &deltaTime)
{
    for (auto &pipe : pipes_)
    {
        pipe.first.move(PIPE_SPEED * deltaTime, 0);
        pipe.second.move(PIPE_SPEED * deltaTime, 0);
    }
}

const Pipes_t &Pipes::getPipes() const
{
    return pipes_;
}

const float Pipes::getPipeSpeed() const
{
    return PIPE_SPEED;
}

const bool Pipes::offScreen(const size_t &i) const
{
    return pipes_[i].first.getPosition().x + pipes_[i].first.getTextureRect().getSize().x < 0;
}

void Pipes::eraseOffScreenPipe(const size_t &i)
{
    if (offScreen(i))
    {
        pipes_.erase(pipes_.begin() + i);
        pipes_ = Pipes_t(std::move(pipes_));
    }
}

void Pipes::reset()
{
    pipes_.clear();
    pipes_ = Pipes_t(std::move(pipes_));
}

Pipes::~Pipes() {}
