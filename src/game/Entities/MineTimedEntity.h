#pragma once

#include "MineEntity.h"

class MineTimedEntity final
    : public MineEntity
{
public:
    MineTimedEntity(const LiveEntity * owner,
                    MathLib::vec2f position,
                    float explosionDamage,
                    float explosionRadius,
                    float triggerDistance,
                    double timeToExplode,
                    const std::shared_ptr<Physics::Entity>& physicsEntity);
    void Update(double dt) override;

private:
    double timeToExplode = 0.0;

};
