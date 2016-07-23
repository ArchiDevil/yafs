#pragma once

#include "Entity.h"

#include <MathLib/math.h>

class Player
    : public Entity
{
public:
    Player(const MathLib::vec2f & position);
    virtual ~Player() = default;

    bool handleEvent(const ProjectilePositionEvent & event) override;
    void Update(double dt) override;
    void Shoot(const MathLib::vec2f & targetPosition);
};
