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
void LiveEntity::Shoot(const vec2f & direction)
{
    vec2f dir = direction;
    float angleFactor = (float)(rand() % 30 - 15) / 100.0f;
    dir = vec2Transform(dir, matrixRotationZ(angleFactor));
    GetGamePtr()->GetEntityMgr()->CreateProjectile(position, dir * 2.0f, 1.0f, 3.0, this);
}

MathLib::vec2f LiveEntity::GetTargetDirection() const
{
    return targetDirection;
}

void LiveEntity::SetTargetDirection(const MathLib::vec2f & val)
{
    targetDirection = val;
}

int LiveEntity::GetExperienceCount()
{
    return experienceCount;
}
