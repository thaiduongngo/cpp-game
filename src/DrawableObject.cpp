#include "DrawableObject.h"

namespace game::common
{
    DrawableObject::DrawableObject() {}

    DrawableObject::DrawableObject(
        const sf::Texture &texture,
        const float &x, const float &y)
    {
        setTexture(texture);
        setPosition(x, y);
    }

    DrawableObject::DrawableObject(const sf::Vector2i &vec,
                                   const sf::Texture &texture,
                                   const float &x,
                                   const float &y)
    {
        setTexture(texture);
        setTextureRect(sf::IntRect(0, 0, vec.x, vec.y));
        setPosition(x, y);
    }

    const sf::Vector2i DrawableObject::getSize() const
    {
        return this->getTextureRect().getSize();
    }

    DrawableObject::~DrawableObject() {}
}
