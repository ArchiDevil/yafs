#include "AIStateDodge.h"

#include "../../Game.h"
#include "../../Entities/EntityManager.h"
#include <MathLib/math.h>

using namespace GoingHome;
using namespace MathLib;

AIStateDodge::AIStateDodge(float dodgeDistance, float dodgeDistanceToProjectileLine)
    : dodgeDistance(dodgeDistance)
    , dodgeDistanceToProjectileLine(dodgeDistanceToProjectileLine)
{
}

void AIStateDodge::Do(double dt, LiveEntity* entity)
{

    auto projectiles = GetGamePtr()->GetEntityMgr()->GetProjectiles();

    Projectile* nearestProjectile = nullptr;
    float nearestDistance = std::numeric_limits<float>::max();

    for (auto& projectile : projectiles)
    {
        if (projectile->GetProducer() != entity)
        {
            float distanceToProjectile = distance(entity->GetPosition(), projectile->GetPosition());
            if (distanceToProjectile < nearestDistance && distanceToProjectile < dodgeDistance)
            {
                nearestProjectile = projectile;
                nearestDistance = distanceToProjectile;
            }
        }
    }

    if (!nearestProjectile)
        return;

    vec2f position = entity->GetPosition();
    vec2f projectileSpeed = normalize(nearestProjectile->GetSpeed());

    auto projectileLine = lineFromPointAndDirectionVector(nearestProjectile->GetPosition(), projectileSpeed);
    float distanceToProjectileLine = distanceFromPointToLine(projectileLine, position);
    if (distanceToProjectileLine < dodgeDistanceToProjectileLine)
    {
        auto projection = projectLineOnPoint(position, projectileLine);
        vec2f dodgeVector = position - projection;
        position += normalize(dodgeVector) * (dt / 2.0);
        entity->SetPosition(position);
    }
}
