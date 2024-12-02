#include "Mounts.h"

namespace game::mount
{
    Mounts::Mounts() : mounts_()
    {
        for (int i = 0; i < NUMBER_OF_MOUNTS; i++)
        {
            const std::string image_ = std::format("./res/Mount{}.png", i);
            // @todo handle error no file found here
            textures_[i].loadFromFile(image_);
            textures_[i].setSmooth(true);
        }
    }

    void Mounts::spawnMount(const float &start, const int &height, const float &deltaTime)
    {
        mountSpawnTimer += deltaTime;
        if (mountSpawnTimer >= MOUNT_SPAWN_INTERVAL)
        {
            mountSpawnTimer = 0.f;
            std::srand(std::time(nullptr));
            const int i = (std::rand() % 3); // Randomize mount
            mounts_.emplace_back(std::make_unique<Mount>(textures_[i], start, height - textures_[i].getSize().y));
        }
    }

    void Mounts::moveMount(const float &deltaTime)
    {
        for (auto &mount : mounts_)
        {
            mount->move(MOUNT_SPEED * deltaTime, 0);
        }
    }

    const Mounts_t &Mounts::getMounts() const
    {
        return mounts_;
    }

    const bool Mounts::offScreen(const size_t &i) const
    {
        return mounts_[i]->getPosition().x + mounts_[i]->getTextureRect().getSize().x < 0;
    }

    void Mounts::eraseOffScreenMount()
    {
        for (int i = 0; i < mounts_.size(); i++)
        {
            if (offScreen(i))
            {
                mounts_.erase(mounts_.begin() + i);
            }
        }
    }

    void Mounts::reset()
    {
        mountSpawnTimer = 0.f;
        mounts_.clear();
        Mounts_t().swap(mounts_);
    }

    Mounts::~Mounts() {}
}
