#include "AIStatePursuit.h"

#include "../../Game.h"
#include "../../Entities/EntityManager.h"

#include <MathLib/math.h>

using namespace GoingHome;
using namespace MathLib;

void AIStatePursuit::Do(double dt, LiveEntity* entity)
{
    auto hostileEnemies = GetGamePtr()->GetEntityMgr()->GetHostileLiveEntities(entity->GetFaction());

    LiveEntity* nearestEnemy = nullptr;
    float nearestDistance = std::numeric_limits<float>::max();

    for (auto& enemy : hostileEnemies)
    {
        float enemyDistance = distance(entity->GetPosition(), enemy->GetPosition());
        if (enemyDistance < nearestDistance)
        {
            nearestEnemy = enemy;
            nearestDistance = enemyDistance;
        }
    }

    if (!nearestEnemy)
        return;

    // we assume that entity is behind border of pursuiting
    vec2f directionVector = normalize(nearestEnemy->GetPosition() - entity->GetPosition());
    if (directionVector.length())
    {
        vec2f position = entity->GetPosition();
        position += normalize(directionVector) * (dt / 2.0);
        entity->SetPosition(position);
    }
}
