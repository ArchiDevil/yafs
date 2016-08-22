#pragma once

#include "Entity.h"

#include <MathLib/math.h>

class Player
    : public Entity
{
public:
    Player(const MathLib::vec2f & position, float health);
    virtual ~Player() = default;

    bool handleEvent(const ProjectilePositionEvent & event) override;
    void Update(double dt) override;
    void Shoot(const MathLib::vec2f & targetPosition);
    void ShootAlternative(const MathLib::vec2f & targetPosition);
    void SetMoveVelocity(const MathLib::vec2f &velocity);

private:
    MathLib::vec2f moveVelocity = {};
    float health = 1.0f;

};
