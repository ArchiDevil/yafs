#include "AISmallSpirit.h"

#include "States\AIStateEscape.h"

AISmallSpirit::AISmallSpirit()
    : AIBase()
{

}

void AISmallSpirit::Update(double dt, LiveEntity * entity)
{
    switch (currentState)
    {
    case AIStateType::Idle:
        if (entity->GetHealth() / entity->GetMaxHealth() < 0.3)
        {
            currentAction = std::make_shared<AIStateEscape>();
            currentState = AIStateType::Escape;
        }
        break;
    }

    if (currentAction != nullptr)
    {
        currentAction->Do(dt, entity);
    }
}