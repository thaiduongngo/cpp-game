#pragma once
#include <SFML/Graphics.hpp>

namespace game::mount
{
    class Mount : public sf::Sprite
    {
    public:
        Mount(const sf::Texture &texture, const float &x, const float &y);
        ~Mount();
    };
}
