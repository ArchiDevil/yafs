#include "MineDetectorEntity.h"

#include "../Game.h"

#include "LiveEntity.h"

MineDetectorEntity::MineDetectorEntity(const LiveEntity * owner,
                                       MathLib::vec2f position,
                                       float explosionDamage,
                                       float explosionRadius,
                                       float triggerDistance,
                                       const std::shared_ptr<Physics::Entity>& physicsEntity)
    : MineEntity(owner, position, explosionDamage, explosionRadius, triggerDistance, physicsEntity)
{
}

bool MineDetectorEntity::handleEvent(const LiveEntityPositionEvent & event)
{
    if (event.producer == MineEntity::owner)
        return true;

    if (MathLib::distance(Entity::GetPosition(), event.producer->GetPosition()) < MineEntity::triggerDistance)
    {
        MineEntity::Explode();
        return false;
    }

    return true;
}
