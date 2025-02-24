#pragma once
#include <SFML/Graphics.hpp>

namespace game::common
{
    class DrawableObject : public sf::Sprite
    {
    public:
        DrawableObject(const sf::Texture &texture);
        DrawableObject(const sf::Texture &texture,
                       const float &x,
                       const float &y);
        DrawableObject(const sf::Vector2i &vec,
                       const sf::Texture &texture,
                       const float &x,
                       const float &y);
        const sf::Vector2u getSize() const;
        ~DrawableObject();
    };
}
