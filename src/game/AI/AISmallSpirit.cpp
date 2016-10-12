#include "AISmallSpirit.h"

#include "Actions\AIActionEscape.h"

void AISmallSpirit::Update(LiveEntity & entity)
{
    switch (currentState)
    {
    case AIBase::AIState::Idle:
        if (entity.GetHealth() / entity.GetMaxHealth() < 0.2)
        {
            currentAction = std::make_shared<AIActionEscape>();
            currentState = AIBase::AIState::Escape;
        }
        break;
    }

    if (currentAction != nullptr)
    {
        currentAction->Do(entity);
    }
}