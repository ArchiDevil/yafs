#pragma once

#include "Entity.h"

#include <MathLib/math.h>

class Projectile : public Entity
{
public:
    Projectile(MathLib::vec2f & position, MathLib::vec2f & speed);
    virtual ~Projectile() { }
    std::string GetName() override;
    void Update(double deltaTime) override;

private:
    
    MathLib::vec2f speed;
};