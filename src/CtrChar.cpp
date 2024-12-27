#include "CtrChar.h"

namespace game::character
{
    CtrChar::CtrChar(const sf::Texture &texture) : DrawableObject(texture)
    {
        // setTexture(texture);
    }

    const float CtrChar::getVelocity() const
    {
        return velocity;
    }

    void CtrChar::jump()
    {
        velocity = JUPMP_DISTANCE;
    }

    void CtrChar::moveAndFall(const float &gravity, const float &deltaTime)
    {
        velocity += gravity * deltaTime;
        move(sf::Vector2f(0, velocity * deltaTime));
    }

    const bool CtrChar::collidedWEdge(const float &top, const float &bottom) const
    {
        const sf::Vector2u size = getSize();
        return getPosition().y < bottom || (getPosition().y + size.y > top);
    }

    const bool CtrChar::collidedWPipe(const game::pipes::PairPipes &pipe) const
    {
        return getGlobalBounds().findIntersection(pipe.top->getGlobalBounds()) ||
               getGlobalBounds().findIntersection(pipe.bottom->getGlobalBounds());
    }

    const bool CtrChar::passedPipe(const game::pipes::PairPipes &pipe, const float &pipeSpeed, const float &deltaTime) const
    {
        return pipe.top->getPosition().x + pipe.top->getSize().x < getPosition().x && pipe.top->getPosition().x + pipe.top->getSize().x > getPosition().x + pipeSpeed * deltaTime;
    }

    void CtrChar::moveToOriginalPosition()
    {
        setPosition(CHAR_ORG_POSITION);
    }

    void CtrChar::reset()
    {
        moveToOriginalPosition();
        velocity = 0.f;
    }

    CtrChar::~CtrChar() {}
}
