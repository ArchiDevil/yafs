#pragma once

#include "Entity.h"

#include <MathLib/math.h>

class Player : public Entity
{
public:
    Player(MathLib::vec2f & position);
    virtual ~Player() { }
    std::string GetName() override;
    bool handleEvent(const EntityEvent & event) override;

    void Shoot(MathLib::vec2f & targetPosition);
};