#pragma once

#include "Entity.h"

#include <MathLib/math.h>

class Projectile
    : public Entity
{
public:
    Projectile(const MathLib::vec2f & position, const MathLib::vec2f & speed, Entity * producer);
    virtual ~Projectile() = default;
    void Update(double dt) override;
    Entity* GetProducer() const;

private:
    MathLib::vec2f speed;
    double lifetime = 0.0;
    Entity* producer = nullptr;

};
