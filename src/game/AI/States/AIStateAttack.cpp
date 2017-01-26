#include "AIStateAttack.h"

#include "../../Game.h"
#include "../../Entities/EntityManager.h"

using namespace GoingHome;
using namespace MathLib;

AIStateAttack::AIStateAttack(LiveEntity * entity)
    : entity(entity)
{
    if (!entity)
        throw std::invalid_argument("entity is nullptr");
}

AIStateAttack::~AIStateAttack()
{
    ISpellController* controller = entity->GetSpellController(LiveEntity::CS_MainSlot);
    if (controller)
        controller->SpellKeyUp();
}

void AIStateAttack::Do(double /*dt*/, LiveEntity * /*liveEntity*/)
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

    // we assume that entity is behind border of attacking
    vec2f directionVector = normalize(nearestEnemy->GetPosition() - entity->GetPosition());
    entity->SetTargetDirection(directionVector);

    ISpellController* controller = entity->GetSpellController(LiveEntity::CS_MainSlot);
    if (controller)
        controller->SpellKeyDown();
}
