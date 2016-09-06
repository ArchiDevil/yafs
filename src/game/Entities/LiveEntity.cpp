#include "LiveEntity.h"

#include "../Game.h"
#include "Projectile.h"
#include "ExperiencePoint.h"

using namespace MathLib;
using namespace GoingHome;
using namespace ShiftEngine;

LiveEntity::LiveEntity(const vec2f & position, float health, const std::wstring & textureName, int expCount)
    : Entity(position, GetSceneGraph()->AddSpriteNode(textureName))
    , health(health)
    , experienceCount(expCount)
{
}

bool LiveEntity::handleEvent(const ProjectilePositionEvent & event)
{
    if (event.projectile->GetProducer() == this)
        return true;

    if (CalculateCollision(*event.projectile))
    {
        health -= event.projectile->GetDamage();
        event.projectile->Kill();

        if (health <= 0)
        {
            Kill();
            GetGamePtr()->GetEntityMgr()->CreateExperiencePoint(position, experienceCount);
        }
    }

    return true;
}

bool LiveEntity::handleEvent(const ExperiencePointPositionEvent & event)
{
    if (CalculateCollision(*event.expPoint))
    {
        experienceCount += event.expPoint->GetExperienceCount();
        event.expPoint->Kill();
        return false;
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

int LiveEntity::GetExperienceCount()
{
    return experienceCount;
}
