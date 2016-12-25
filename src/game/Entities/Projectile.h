#pragma once

#include "LiveEntity.h"

#include <MathLib/math.h>

class Projectile final
    : public Entity
{
public:
    Projectile(const MathLib::vec2f & position, const MathLib::vec2f & speed, float damage, double lifetime, const LiveEntity * producer);
    virtual ~Projectile() = default;
    void Update(double dt) override;
    const LiveEntity * GetProducer() const;

    float GetDamage() const;
    void SetDamage(float val);

private:
    MathLib::vec2f speed;
    double remainingTime = 0.0;
    float damage = 1.0f;
    const LiveEntity* producer = nullptr;

};
