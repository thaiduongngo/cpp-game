#pragma once
#include <SFML/Graphics.hpp>
#include "Cloud.h"

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

Clouds::Clouds() : clouds(), texture_()
{
    // @todo handle error no file found here
    texture_.loadFromFile(CLOUD_IMAGE);
    texture_.setSmooth(true);
};

void Clouds::spawnCloud(const float &start, const int &height)
{
    const float yPosition = static_cast<float>(abs(std::rand()) % height); // Randomize cloud position
    auto cloud = Cloud(texture_);
    cloud.setPosition(start, yPosition);
    clouds.push_back(std::move(cloud));
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
    clouds.clear();
    clouds = Clouds_t(std::move(clouds));
}

Clouds::~Clouds() {}