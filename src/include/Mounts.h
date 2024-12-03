#pragma once
#include <SFML/Graphics.hpp>
#include "Mount.h"
#include <format>
#include <string>

namespace game::mount
{
    constexpr auto MOUNT_SPEED = -95.f;
    constexpr auto MOUNT_SPAWN_INTERVAL = 5.5f;
    constexpr size_t NUMBER_OF_MOUNTS = 5;

    typedef std::vector<std::unique_ptr<Mount>> Mounts_t;

    class Mounts
    {
    private:
        Mounts_t mounts_;
        std::array<sf::Texture, NUMBER_OF_MOUNTS> textures_;
        float mountSpawnTimer = 0.f;

    public:
        Mounts();
        void spawnMount(const float &start, const int &height, const float &deltaTime);
        void moveMount(const float &deltaTime);
        const Mounts_t &getMounts() const;
        const bool offScreen(const size_t &i) const;
        void eraseOffScreenMount();
        void reset();
        ~Mounts();
    };
}
