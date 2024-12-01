#include "Pipe.h"

namespace game::pipes
{

    Pipe::Pipe(const sf::Vector2f &vec, const sf::Texture &texture, const float &x, const float &y)
    {
        setTexture(texture);
        setTextureRect(sf::IntRect(0, 0, vec.x, vec.y));
        setPosition(x, y);
    }

    const sf::Vector2i Pipe::getSize() const
    {
        return this->getTextureRect().getSize();
    }

    Pipe::~Pipe() {}
}