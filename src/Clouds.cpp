#include "Clouds.h"

namespace game::cloud
{
    Clouds::Clouds() : clouds_()
    {
        clouds_.reserve(CLOUD_RESERVED);
        if (texture_.loadFromFile(CLOUD_IMAGE))
        {
            // @todo handle error no file found here
        }
        texture_.setSmooth(true);
    }

    void Clouds::spawnCloud(const float &start, const int &height, const float &deltaTime)
    {
        cloudSpawnTimer += deltaTime;
        if (cloudSpawnTimer >= CLOUD_SPAWN_INTERVAL)
        {
            cloudSpawnTimer = 0.f;
            std::srand(std::time(nullptr));
            const float yPosition = static_cast<float>(std::rand() % height); // Randomize cloud position
            clouds_.emplace_back(std::make_unique<game::common::DrawableObject>(texture_, start, yPosition));
        }
    }

    void Clouds::moveCloud(const float &deltaTime)
    {
        for (auto &cloud : clouds_)
        {
            cloud->move(sf::Vector2f(CLOUD_SPEED * deltaTime, 0));
        }
    }

    const Clouds_t &Clouds::getClouds() const
    {
        return clouds_;
    }

    const bool Clouds::offScreen(const size_t &i) const
    {
        return clouds_[i]->getPosition().x + clouds_[i]->getTexture().getSize().x < 0;
    }

    void Clouds::eraseOffScreenCloud()
    {
        for (int i = 0; i < clouds_.size(); i++)
        {
            if (offScreen(i))
            {
                clouds_.erase(clouds_.begin() + i);
            }
        }
    }

    void Clouds::reset()
    {
        cloudSpawnTimer = 0.f;
        clouds_.clear();
        Clouds_t().swap(clouds_);
    }

    Clouds::~Clouds() {}
}
