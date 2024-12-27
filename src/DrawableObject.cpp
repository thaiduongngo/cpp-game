#include "DrawableObject.h"

namespace game::common
{
    DrawableObject::DrawableObject(const sf::Texture &texture) : sf::Sprite(texture)
    {
    }

    DrawableObject::DrawableObject(
        const sf::Texture &texture,
        const float &x, const float &y) : sf::Sprite(texture)
    {
        setPosition(sf::Vector2f(x, y));
    }

    DrawableObject::DrawableObject(const sf::Vector2i &vec,
                                   const sf::Texture &texture,
                                   const float &x,
                                   const float &y)
        : sf::Sprite(texture)
    {

        setTextureRect(sf::IntRect(sf::Vector2i(0, 0), vec));
        setPosition(sf::Vector2f(x, y));
    }

    const sf::Vector2u DrawableObject::getSize() const
    {
        return this->getTexture().getSize();
    }

    DrawableObject::~DrawableObject() {}
}
