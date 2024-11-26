#pragma once
#include <SFML/Graphics.hpp>
#include "Cloud.h"

#ifndef CLOUD_SPEED
#define CLOUD_SPEED -150.f
#endif

typedef std::vector<std::unique_ptr<Cloud>> Clouds_t;

class Clouds
{
private:
    std::unique_ptr<Clouds_t> clouds;
    sf::Image image;
    sf::Texture texture;

public:
    Clouds();
    void spawnCloud(const float &start, const int &height);
    void moveCloud(const float &deltaTime);
    const std::unique_ptr<Clouds_t> &getClouds();
    const bool offScreen(const Cloud &cloud) const;
    void eraseCloud(const size_t &i);
    void reset();
    ~Clouds();
};

Clouds::Clouds()
{
    clouds = std::make_unique<Clouds_t>();
    // @todo handle error no file found here
    image.loadFromFile("./res/cloud.png");
    texture.loadFromImage(image);
};

void Clouds::spawnCloud(const float &start, const int &height)
{
    const float yPosition = static_cast<float>(abs(std::rand()) % height); // Randomize cloud position
    auto cloud = std::make_unique<Cloud>(texture);
    cloud->setPosition(start, yPosition);
    (*clouds).push_back(std::move(cloud));
}

void Clouds::moveCloud(const float &deltaTime)
{
    for (auto &cloud : *(clouds))
    {
        cloud->move(CLOUD_SPEED * deltaTime, 0);
    }
}

const std::unique_ptr<Clouds_t> &Clouds::getClouds()
{
    return clouds;
}

const bool Clouds::offScreen(const Cloud &cloud) const
{
    return cloud.getPosition().x + cloud.getTextureRect().getSize().x < 0;
}

void Clouds::eraseCloud(const size_t &i)
{
    (*clouds).erase((*clouds).begin() + i);
    (*clouds).shrink_to_fit();
}

void Clouds::reset()
{
    (*clouds).clear();
    (*clouds).shrink_to_fit();
}

Clouds::~Clouds()
{
}