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

void MineEntity::TakeDamage(float damageCount)
{
    // immediately without any hit points calculation
    // this may be subject of change after some refactoring
    // may be mines should have some hit points for unifying with other entities
    Explode();
}

void MineEntity::Explode()
{
    ((notifier<ExplosionEvent>)EntityEventManager::GetInstance())
        .notifyAll(ExplosionEvent(Entity::position, explosionDamage, explosionRadius));
    Kill();
}
