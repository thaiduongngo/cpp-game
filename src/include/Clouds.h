#pragma once
#include <SFML/Graphics.hpp>
#include "Cloud.h"

namespace game::cloud
{
    constexpr auto CLOUD_SPEED = -100.f;
    constexpr auto CLOUD_IMAGE = "./res/cloud.png";

    typedef std::vector<Cloud> Clouds_t;

    class Clouds
    {
    private:
        Clouds_t clouds;
        sf::Texture texture_;

    public:
        Clouds();
        void spawnCloud(const float &start, const int &height);
        void moveCloud(const float &deltaTime);
        const Clouds_t &getClouds() const;
        const bool offScreen(const size_t &i) const;
        void eraseOffScreenCloud();
        void reset();
        ~Clouds();
    };
}
