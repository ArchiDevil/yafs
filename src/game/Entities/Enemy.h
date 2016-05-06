#pragma once

#include "Entity.h"

#include <MathLib/math.h>

class Enemy : public Entity
{
public:
    Enemy(MathLib::vec2f & position);
    virtual ~Enemy() { }
    bool handleEvent(const ProjectilePositionEvent & event) override;
    void Update(double dt) override {}

};