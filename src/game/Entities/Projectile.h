#pragma once

#include "Entity.h"

#include <MathLib/math.h>

class Projectile
    : public Entity
{
public:
    Projectile(const MathLib::vec2f & position, const MathLib::vec2f & speed, float damage, double lifetime, Entity * producer);
    virtual ~Projectile() = default;
    void Update(double dt) override;
    Entity* GetProducer() const;

    float GetDamage() const;
    void SetDamage(float val);

private:
    MathLib::vec2f speed;
    double remainingTime = 0.0;
    float damage = 1.0f;
    Entity* producer = nullptr;

};
