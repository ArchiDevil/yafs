#pragma once

#include "Entity.h"

#include <MathLib/math.h>

class Projectile
    : public Entity
{
public:
    Projectile(MathLib::vec2f & position, MathLib::vec2f & speed);
    virtual ~Projectile() { }
    void Update(double dt) override;

private:

    MathLib::vec2f speed;
};