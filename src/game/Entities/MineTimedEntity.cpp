#include "MineTimedEntity.h"

#include "../Game.h"

MineTimedEntity::MineTimedEntity(const LiveEntity * owner,
                                 const MathLib::vec2f& position,
                                 float explosionDamage,
                                 float explosionRadius,
                                 float triggerDistance,
                                 double timeToExplode)
    : MineEntity(owner, position, explosionDamage, explosionRadius, triggerDistance)
    , timeToExplode(timeToExplode)
{
}

void MineTimedEntity::Update(double dt)
{
    timeToExplode -= dt;
    if (timeToExplode <= 0.0)
        Explode();
}
