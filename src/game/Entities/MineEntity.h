#pragma once

#include "Entity.h"

#include <Utilities/observer.h>

class MineEntity
    : public Entity
    , observer<ProjectilePositionEvent>
{
public:
    MineEntity(const LiveEntity * owner, const MathLib::vec2f& position, float explosionDamage, float explosionRadius, float triggerDistance);
    bool observer<ProjectilePositionEvent>::handleEvent(const ProjectilePositionEvent & event) override;

protected:
    void Explode();

    scoped_subscriber<ProjectilePositionEvent> projectileSubscriber = {&EntityEventManager::GetInstance(), this};
    const LiveEntity * owner = nullptr;
    const float explosionDamage = 1.0f;
    const float explosionRadius = 1.0f;
    const float triggerDistance = 1.0f;

};
