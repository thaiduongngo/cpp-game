#pragma once
#include <SFML/Graphics.hpp>

class Cloud : public sf::Sprite
{
public:
    Cloud(sf::Texture &texture);
    ~Cloud();
};

Cloud::Cloud(sf::Texture &texture)
{
    setTexture(texture);
}

Cloud::~Cloud()
{
}
