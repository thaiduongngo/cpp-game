#pragma once
#include <SFML/Graphics.hpp>

namespace game::pipes
{
    class Pipe : public sf::Sprite
    {
    public:
        Pipe(const sf::Vector2f &vec, const sf::Texture &texture, const float &x, const float &y);
        const sf::Vector2i getSize() const;
        ~Pipe();
    };
}
