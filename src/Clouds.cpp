#include "Clouds.h"

namespace game::cloud
{
    Clouds::Clouds() : clouds()
    {
        // @todo handle error no file found here
        texture_.loadFromFile(CLOUD_IMAGE);
        texture_.setSmooth(true);
    }

    void Clouds::spawnCloud(const float &start, const int &height, const float &deltaTime)
    {
        cloudSpawnTimer += deltaTime;
        if (cloudSpawnTimer >= CLOUD_SPAWN_INTERVAL)
        {
            cloudSpawnTimer = 0.f;
            const float yPosition = static_cast<float>(abs(std::rand()) % height); // Randomize cloud position
            auto cloud = Cloud(texture_);
            cloud.setPosition(start, yPosition);
            clouds.push_back(std::move(cloud));
        }
    }

    void Clouds::moveCloud(const float &deltaTime)
    {
        for (auto &cloud : clouds)
        {
            cloud.move(CLOUD_SPEED * deltaTime, 0);
        }
    }

    const Clouds_t &Clouds::getClouds() const
    {
        return clouds;
    }

    const bool Clouds::offScreen(const size_t &i) const
    {
        return clouds[i].getPosition().x + clouds[i].getTextureRect().getSize().x < 0;
    }

    void Clouds::eraseOffScreenCloud()
    {
        for (int i = 0; i < clouds.size(); i++)
        {
            if (offScreen(i))
            {
                clouds.erase(clouds.begin() + i);
                clouds = Clouds_t(std::move(clouds));
            }
        }
    }

    void Clouds::reset()
    {
        cloudSpawnTimer = 0.f;
        clouds.clear();
        clouds = Clouds_t(std::move(clouds));
    }

    Clouds::~Clouds() {}
}
