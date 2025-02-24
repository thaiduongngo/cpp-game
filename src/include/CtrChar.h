#pragma once

#include <SFML/Graphics.hpp>
#include "Pipes.h"
#include "DrawableObject.h"

#ifndef CHAR_ORG_POSITION
#define CHAR_ORG_POSITION sf::Vector2f(100.f, 300.f)
#endif

namespace game::character
{
    constexpr auto JUPMP_DISTANCE = -400.f;

    class CtrChar : public game::common::DrawableObject
    {
    private:
        sf::Texture texture_;
        float velocity = 0.f;

    public:
        CtrChar(const sf::Texture &texture);
        const float getVelocity() const;
        void moveToOriginalPosition();
        void jump();
        void moveAndFall(const float &gravity, const float &deltaTime);
        const bool collidedWEdge(const float &top, const float &bottom) const;
        const bool collidedWPipe(const game::pipes::PairPipes &pipe) const;
        const bool passedPipe(const game::pipes::PairPipes &pipe, const float &pipeSpeed, const float &deltaTime) const;
        void reset();
        ~CtrChar();
    };
}
