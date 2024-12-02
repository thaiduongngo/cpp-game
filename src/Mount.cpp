#include "Mount.h"

namespace game::mount
{
    Mount::Mount(const sf::Texture &texture, const float &x, const float &y)
    {
        setTexture(texture);
        setPosition(x, y);
    }

    Mount::~Mount() {}
}
