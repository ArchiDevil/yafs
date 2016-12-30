#include "MineTimedEntity.h"

#include "../Game.h"

MineTimedEntity::MineTimedEntity(const LiveEntity * owner,
                                 MathLib::vec2f position,
                                 float explosionDamage,
                                 float explosionRadius,
                                 float triggerDistance,
                                 double timeToExplode,
                                 const std::shared_ptr<Physics::Entity>& physicsEntity)
    : MineEntity(owner, position, explosionDamage, explosionRadius, triggerDistance, physicsEntity)
    , timeToExplode(timeToExplode)
{
}

void MineTimedEntity::Update(double dt)
{
    MineEntity::Update(dt);

    timeToExplode -= dt;
    if (timeToExplode <= 0.0)
        Explode();
}
