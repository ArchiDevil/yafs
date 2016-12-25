#include "MineEntity.h"

#include "../Game.h"

const std::wstring mineSpriteName = L"mine.png";

using namespace ShiftEngine;

MineEntity::MineEntity(const LiveEntity * owner,
                       const MathLib::vec2f& position,
                       float explosionDamage,
                       float explosionRadius,
                       float triggerDistance)
    : Entity(position, GetSceneGraph()->AddSpriteNode(mineSpriteName))
    , owner(owner)
    , explosionDamage(explosionDamage)
    , explosionRadius(explosionRadius)
    , triggerDistance(triggerDistance)
{
}

bool MineEntity::handleEvent(const ProjectilePositionEvent & event)
{
    if (CalculateCollision(*event.projectile))
    {
        Explode();
        return false;
    }
    return true;
}

void MineEntity::Explode()
{
    ((notifier<ExplosionEvent>)EntityEventManager::GetInstance())
        .notifyAll(ExplosionEvent(Entity::position, explosionDamage, explosionRadius));
    Kill();
}
