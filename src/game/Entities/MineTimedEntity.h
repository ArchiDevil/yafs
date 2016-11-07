#pragma once

#include "MineEntity.h"

class MineTimedEntity final
    : public MineEntity
{
public:
    MineTimedEntity(const LiveEntity * owner,
                    const MathLib::vec2f& position,
                    float explosionDamage,
                    float explosionRadius,
                    float triggerDistance,
                    double timeToExplode);
    void Update(double dt) override;

private:
    double timeToExplode = 0.0;

};
