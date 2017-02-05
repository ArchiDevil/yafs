#pragma once

#include "PhysicsEntity.h"

#include <PhysicsEngine/PhysicsEngine.h>
#include <Utilities/observer.h>

class MineEntity
    : public PhysicsEntity
{
public:
    MineEntity(const LiveEntity * owner,
               const MathLib::vec2f& position,
               float explosionDamage,
               float explosionRadius,
               float triggerDistance,
               const std::shared_ptr<Physics::Entity>& physicsEntity);
    virtual ~MineEntity() = default;

    void TakeDamage(float damageCount) override;

protected:
    void Explode();

    const LiveEntity * owner = nullptr;
    const float explosionDamage = 1.0f;
    const float explosionRadius = 1.0f;
    const float triggerDistance = 1.0f;

};
