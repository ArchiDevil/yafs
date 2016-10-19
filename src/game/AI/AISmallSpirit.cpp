#include "AISmallSpirit.h"

#include "Actions\AIActionEscape.h"

AISmallSpirit::AISmallSpirit()
    : AIBase()
{

}

void AISmallSpirit::Update(double dt, LiveEntity * entity)
{
    switch (currentState)
    {
    case AIBase::AIState::Idle:
        if (entity->GetHealth() / entity->GetMaxHealth() < 0.3)
        {
            currentAction = std::make_shared<AIActionEscape>();
            currentState = AIBase::AIState::Escape;
        }
        break;
    }

    if (currentAction != nullptr)
    {
        currentAction->Do(dt, entity);
    }
}