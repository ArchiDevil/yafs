#include "LiveEntity.h"

#include "../Game.h"

using namespace MathLib;
using namespace GoingHome;
using namespace ShiftEngine;

LiveEntity::LiveEntity(const vec2f & position, float health, const std::wstring & textureName)
    : Entity(position, GetSceneGraph()->AddSpriteNode(textureName))
{
}

bool LiveEntity::handleEvent(const ProjectilePositionEvent & event)
{
    if (event.projectile->GetProducer() == this)
        return false;

    if (CalculateCollision(*event.projectile))
    {
        health -= event.projectile->GetDamage();
        event.projectile->Kill();

        if (health <= 0)
            Kill();
    }

    return true;
}

// TODO: use direction here
void LiveEntity::Shoot(const vec2f & targetPosition)
{
    auto direction = normalize(targetPosition - position);
    float angleFactor = (float)(rand() % 30 - 15) / 100.0f;
    direction = vec2Transform(direction, matrixRotationZ(angleFactor));
    GetGamePtr()->GetEntityMgr()->CreateProjectile(position, direction * 2.0f, 1.0f, 3.0, this);
}
