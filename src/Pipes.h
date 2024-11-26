#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

#ifndef PIPE_MAX_WIDTH
#define PIPE_MAX_WIDTH 110.f
#endif

#ifndef PIPE_MAX_HEIGHT
#define PIPE_MAX_HEIGHT 600.f
#endif

#ifndef PIPE_GAP
#define PIPE_GAP 270.f
#endif

#ifndef PIPE_SPEED
#define PIPE_SPEED -200.f
#endif

#ifndef PIPE_COLOR
#define PIPE_COLOR sf::Color::Yellow
#endif

#ifndef PIPE_OUTLINE_COLOR
#define PIPE_OUTLINE_COLOR sf::Color::Red
#endif

typedef sf::RectangleShape Pipe_t;
typedef std::pair<sf::RectangleShape, sf::RectangleShape> PairPipe_t;
typedef std::vector<PairPipe_t> Pipes_t;

class Pipes
{
private:
    float pipeSpeed = PIPE_SPEED;
    Pipes_t pipes;

public:
    Pipes();
    void spawnPipe(const float &start, const int &range);
    void movePipes(const float &deltaTime);

    const Pipes_t &getPipes() const
    {
        return pipes;
    }

    const float getPipeSpeed() const
    {
        return pipeSpeed;
    }

    const bool offScreen(const PairPipe_t &pipe) const;

    void erasePipe(const size_t &i)
    {
        pipes.erase(pipes.begin() + i);
        pipes.shrink_to_fit();
    }

    void reset()
    {
        pipes.clear();
        pipes.shrink_to_fit();
    }

    ~Pipes();
};

Pipes::Pipes() : pipes()
{
}

void Pipes::spawnPipe(const float &start, const int &range)
{
    const float yPosition = static_cast<float>(abs(std::rand()) % range); // Randomize pipe position
    auto pipeUp = Pipe_t(sf::Vector2f(PIPE_MAX_WIDTH, PIPE_MAX_HEIGHT));
    auto pipeDown = Pipe_t(sf::Vector2f(PIPE_MAX_WIDTH, PIPE_MAX_HEIGHT));
    pipeUp.setFillColor(PIPE_COLOR);
    pipeUp.setOutlineColor(PIPE_OUTLINE_COLOR);
    pipeUp.setOutlineThickness(1.5f);
    pipeDown.setFillColor(PIPE_COLOR);
    pipeDown.setOutlineColor(PIPE_OUTLINE_COLOR);
    pipeDown.setOutlineThickness(1.5f);
    pipeUp.setPosition(start, yPosition - PIPE_MAX_HEIGHT);
    pipeDown.setPosition(start, yPosition + PIPE_GAP);
    pipes.push_back({pipeUp, pipeDown});
}

void Pipes::movePipes(const float &deltaTime)
{
    for (auto &pipe : pipes)
    {
        pipe.first.move(pipeSpeed * deltaTime, 0);
        pipe.second.move(pipeSpeed * deltaTime, 0);
    }
}

const bool Pipes::offScreen(const PairPipe_t &pipe) const
{
    return pipe.first.getPosition().x + pipe.first.getSize().x < 0;
}

Pipes::~Pipes()
{
}
