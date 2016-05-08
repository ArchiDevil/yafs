#pragma once

#include "Entity.h"

#include <MathLib/math.h>

class Player
    : public Entity
{
public:
    Player(MathLib::vec2f & position);
    virtual ~Player() { }
    bool handleEvent(const ProjectilePositionEvent & event) override;
    void Update(double dt) override {}

    void Shoot(MathLib::vec2f & targetPosition);
};