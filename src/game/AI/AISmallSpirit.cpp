#include "AISmallSpirit.h"

#include "States/AIStateAttack.h"
#include "States/AIStateDeath.h"
#include "States/AIStateDodge.h"
#include "States/AIStateEscape.h"
#include "States/AIStateIdle.h"
#include "States/AIStatePursuit.h"

#include "../Game.h"
#include "../Entities/EntityManager.h"

using namespace GoingHome;
using namespace MathLib;

AISmallSpirit::AISmallSpirit(float lowHealthThresholdRatio)
    : lowHealthThresholdRatio(lowHealthThresholdRatio)
{
    currentAction = std::make_unique<AIStateIdle>();
}

void AISmallSpirit::Update(double dt, LiveEntity * entity)
{
    const float pursuitDistance = 4.0f;
    const float attackDistance = 2.0f;
    const float dodgeDistance = 1.0f;
    const float dodgeDistanceToProjectileLine = 0.6f;

    static double elapsedTime = 0.0;
    elapsedTime += dt;

    if (elapsedTime > 0.1)
    {
        EntityManager* entityMgr = GetGamePtr()->GetEntityMgr();
        float healthRatio = entity->GetHealth() / entity->GetMaxHealth();
        bool pursuitTargetFound = false;
        bool attackTargetFound = false;
        bool dangerousProjectilesFound = false;

        for (auto& enemy : entityMgr->GetHostileLiveEntities(entity->GetFaction()))
        {
            float distanceToEnemy = distance(entity->GetPosition(), enemy->GetPosition());
            if (distanceToEnemy < attackDistance)
            {
                attackTargetFound = true;
                break;
            }

            if (distanceToEnemy < pursuitDistance)
            {
                pursuitTargetFound = true;
                break;
            }
        }

        for (auto& projectile : entityMgr->GetProjectiles())
        {
            if (projectile->GetProducer() != entity)
            {
                float distanceToProjectile = distance(entity->GetPosition(), projectile->GetPosition());
                if (distanceToProjectile < dodgeDistance)
                {
                    auto projectileLine = lineFromPointAndDirectionVector(projectile->GetPosition(), projectile->GetSpeed());
                    float distanceToProjectileLine = distanceFromPointToLine(projectileLine, entity->GetPosition());
                    if (distanceToProjectileLine < dodgeDistanceToProjectileLine)
                    {
                        dangerousProjectilesFound = true;
                        break;
                    }
                }
            }
        }

        // check conditions and select new state if it is applicable
        if (healthRatio < lowHealthThresholdRatio)
        {
            setAction<AIStateEscape>(pursuitDistance);
        }
        else if (dangerousProjectilesFound)
        {
            setAction<AIStateDodge>(dodgeDistance, dodgeDistanceToProjectileLine);
        }
        else if (attackTargetFound)
        {
            setAction<AIStateAttack>(entity);
        }
        else if (pursuitTargetFound)
        {
            setAction<AIStatePursuit>();
        }
        else if (healthRatio <= 0.0)
        {
            setAction<AIStateDeath>();
        }
        else
        {
            if (currentAction->GetType() != AIStateType::Idle &&
                currentAction->GetType() != AIStateType::Death)
            {
                currentAction = std::make_unique<AIStateIdle>();
#ifdef _DEBUG
                printState(AIStateType::Idle);
#endif
            }
        }
        elapsedTime = 0.0;
    }

    currentAction->Do(dt, entity);
}
