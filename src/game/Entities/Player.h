#pragma once

#include "LiveEntity.h"

#include <MathLib/math.h>

class Player final
    : public LiveEntity
{
public:
    Player(const MathLib::vec2f & position, float health);
    virtual ~Player() = default;

    void Update(double dt) override;
    void SetMoveVelocity(const MathLib::vec2f &velocity);

private:
    MathLib::vec2f moveVelocity = {};

};
