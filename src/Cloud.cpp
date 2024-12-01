#include "Cloud.h"

namespace game::cloud
{
    Cloud::Cloud(const sf::Texture &texture, const float &x, const float &y)
    {
        setTexture(texture);
        setPosition(x, y);
    }

    Cloud::~Cloud() {}
}
