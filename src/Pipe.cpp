#include "Pipe.h"

namespace game::pipes
{

    Pipe::Pipe(const sf::Vector2f &vec, const sf::Texture &texture)
    {
        setTexture(texture);
        this->setTextureRect(sf::IntRect(0, 0, vec.x, vec.y));
    }

    const sf::Vector2i Pipe::getSize() const
    {
        return this->getTextureRect().getSize();
    }

    Pipe::~Pipe() {}
}