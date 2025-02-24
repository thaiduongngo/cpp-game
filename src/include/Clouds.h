#pragma once
#include <SFML/Graphics.hpp>
#include "DrawableObject.h"

namespace game::cloud
{
    constexpr float CLOUD_SPEED = -100.f;
    constexpr float CLOUD_SPAWN_INTERVAL = 1.5f;
    constexpr std::string CLOUD_IMAGE = "./res/cloud.png";
    constexpr size_t CLOUD_RESERVED = 12;

    using Clouds_t = std::vector<std::unique_ptr<game::common::DrawableObject>>;

    class Clouds
    {
    private:
        Clouds_t clouds_;
        sf::Texture texture_;
        float cloudSpawnTimer = 0.f;

    public:
        Clouds();
        void spawnCloud(const float &start, const int &height, const float &deltaTime);
        void moveCloud(const float &deltaTime);
        const Clouds_t &getClouds() const;
        const bool offScreen(const size_t &i) const;
        void eraseOffScreenCloud();
        void reset();
        ~Clouds();
    };
}
