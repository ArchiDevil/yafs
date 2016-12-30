#include "AISmallSpirit.h"

#include "States/AIStateEscape.h"

AISmallSpirit::AISmallSpirit(float lowHealthThresholdRation)
    : lowHealthThresholdRation(lowHealthThresholdRation)
{
}

void AISmallSpirit::Update(double dt, LiveEntity * entity)
{
    switch (currentState)
    {
    case AIStateType::Idle:
        if (entity->GetHealth() / entity->GetMaxHealth() < lowHealthThresholdRation)
        {
            currentAction = std::make_shared<AIStateEscape>();
            currentState = AIStateType::Escape;
        }
        break;
    }

    if (currentAction)
        currentAction->Do(dt, entity);
}
