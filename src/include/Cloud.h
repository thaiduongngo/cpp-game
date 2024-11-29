#pragma once
#include <SFML/Graphics.hpp>

class Cloud : public sf::Sprite
{
public:
    Cloud(const sf::Texture &texture);
    ~Cloud();
};

Cloud::Cloud(const sf::Texture &texture)
{
    setTexture(texture);
}

Cloud::~Cloud() {}
