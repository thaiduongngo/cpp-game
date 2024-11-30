#include "Cloud.h"

namespace game::cloud
{
    Cloud::Cloud(const sf::Texture &texture)
    {
        setTexture(texture);
    }

    Cloud::~Cloud() {}
}
