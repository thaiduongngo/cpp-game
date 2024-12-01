#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Pipe.h"

#ifndef PIPE_COLOR
#define PIPE_COLOR sf::Color::Yellow
#endif

#ifndef PIPE_OUTLINE_COLOR
#define PIPE_OUTLINE_COLOR sf::Color::Red
#endif

namespace game::pipes
{
    constexpr auto PIPE_MAX_WIDTH = 110.f;
    constexpr auto PIPE_MAX_HEIGHT = 660.f;
    constexpr auto PIPE_GAP = 270.f;
    constexpr auto PIPE_SPEED = -220.f;
    constexpr auto PIPE_SPAWN_INTERVAL = 1.75f;
    constexpr auto PIPE_IMAGE = "./res/stripes.png";
    constexpr auto PIPE_RESERVED = 5;

    typedef Pipe Pipe_t;
    typedef std::pair<Pipe_t, Pipe_t> PairPipe_t;
    typedef std::vector<PairPipe_t> Pipes_t;

    class Pipes
    {
    private:
        Pipes_t pipes_;
        sf::Texture texture_;
        float pipeSpawnTimer = 0.f;

    public:
        Pipes();
        void spawnPipe(const float &start, const int &range, const float &deltaTime);
        void movePipes(const float &deltaTime);
        const Pipes_t &getPipes() const;
        const float getPipeSpeed() const;
        const bool offScreen(const size_t &i) const;
        void eraseOffScreenPipe(const size_t &i);
        void reset();
        ~Pipes();
    };
}
