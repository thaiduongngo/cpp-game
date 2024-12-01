#pragma once
#include <SFML/Graphics.hpp>

namespace game::cloud
{
    class Cloud : public sf::Sprite
    {
    public:
        Cloud(const sf::Texture &texture);
        Cloud(const sf::Texture &texture, const float &x, const float &y);
        ~Cloud();
    };
}
