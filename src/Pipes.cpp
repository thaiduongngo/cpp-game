#include "Pipes.h"

namespace game::pipes
{
    Pipes::Pipes() : pipes_()
    {
        pipes_.reserve(PIPE_RESERVED);
        texture_.loadFromFile(PIPE_IMAGE);
        texture_.setSmooth(true);
        texture_.setRepeated(true);
    }

    void Pipes::spawnPipe(const float &start, const int &range, const float &deltaTime)
    {
        pipeSpawnTimer += deltaTime;
        if (pipeSpawnTimer >= PIPE_SPAWN_INTERVAL)
        {
            pipeSpawnTimer = 0.f;
            const auto vecPipe = sf::Vector2f(PIPE_MAX_WIDTH, PIPE_MAX_HEIGHT);
            const float yPosition = static_cast<float>(abs(std::rand()) % range); // Randomize pipe position
            auto pipeUp = Pipe_t(vecPipe, texture_);
            auto pipeDown = Pipe_t(vecPipe, texture_);
            pipeUp.setPosition(start, yPosition - PIPE_MAX_HEIGHT);
            pipeDown.setPosition(start, yPosition + PIPE_GAP);
            pipes_.emplace_back(PairPipe_t(pipeUp, pipeDown));
        }
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
        }
    }

    void Pipes::reset()
    {
        pipeSpawnTimer = 0.f;
        pipes_.clear();
        pipes_.shrink_to_fit();
    }

    Pipes::~Pipes() {}
}
