#include "MineDetectorEntity.h"

#include "../Game.h"

MineDetectorEntity::MineDetectorEntity(const LiveEntity * owner,
                                       const MathLib::vec2f& position,
                                       float explosionDamage,
                                       float explosionRadius,
                                       float triggerDistance)
    : MineEntity(owner, position, explosionDamage, explosionRadius, triggerDistance)
{
}

void MineDetectorEntity::Update(double /*dt*/)
{
}

bool MineDetectorEntity::handleEvent(const LiveEntityPositionEvent & event)
{
    if (event.producer == MineEntity::owner)
        return true;

    if (MathLib::distance(Entity::position, event.producer->GetPosition()) < MineEntity::triggerDistance)
    {
        MineEntity::Explode();
        return false;
    }

    return true;
}
