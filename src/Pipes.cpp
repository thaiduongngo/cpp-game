#include "Pipes.h"

namespace game::pipes
{
    Pipes::Pipes() : pipes_(), texture_(PIPE_IMAGE), pipeSpawnTimer(0.f)
    {
        pipes_.reserve(PIPE_RESERVED);
        texture_.setSmooth(true);
        texture_.setRepeated(true);
    }

    void Pipes::spawnPipe(const float &start, const int &range, const float &deltaTime)
    {
        pipeSpawnTimer += deltaTime;
        if (pipeSpawnTimer >= PIPE_SPAWN_INTERVAL)
        {
            pipeSpawnTimer = 0.f;
            std::srand(std::time(nullptr));
            const auto vecPipe = sf::Vector2i(PIPE_MAX_WIDTH, PIPE_MAX_HEIGHT);
            const float yPosition = static_cast<float>(std::rand() % range); // Randomize pipe position

            pipes_.emplace_back(PairPipes(
                std::make_unique<Pipe_t>(vecPipe, texture_, start, yPosition - PIPE_MAX_HEIGHT),
                std::make_unique<Pipe_t>(vecPipe, texture_, start, yPosition + PIPE_GAP)));
        }
    }

    void Pipes::movePipes(const float &deltaTime)
    {
        for (auto &pipe : pipes_)
        {
            pipe.top->move(sf::Vector2f(PIPE_SPEED * deltaTime, 0));
            pipe.bottom->move(sf::Vector2f(PIPE_SPEED * deltaTime, 0));
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
        return pipes_[i].top->getPosition().x + pipes_[i].top->getSize().x < 0;
    }

    void Pipes::eraseOffScreenPipe(const size_t &i)
    {
        if (offScreen(i))
        {
            pipes_.erase(pipes_.begin() + i);
        }
    }

    void Pipes::reset()
    {
        pipeSpawnTimer = 0.f;
        pipes_.clear();
        Pipes_t().swap(pipes_);
    }

    Pipes::~Pipes() {}
}
