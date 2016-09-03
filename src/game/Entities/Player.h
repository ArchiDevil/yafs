#pragma once

#include "LiveEntity.h"

#include <MathLib/math.h>

class Player
    : public LiveEntity
{
public:
    Player(const MathLib::vec2f & position, float health);
    virtual ~Player() = default;

    void Update(double dt) override;
    void ShootAlternative(const MathLib::vec2f & targetPosition);
    void SetMoveVelocity(const MathLib::vec2f &velocity);

private:
    MathLib::vec2f moveVelocity = {};

};
