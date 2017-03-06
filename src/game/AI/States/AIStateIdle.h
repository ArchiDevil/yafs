#pragma once

#include "AIState.h"

class AIStateIdle final
    : public AIState
{
public:
    void Do(double /*dt*/, LiveEntity * entity) override
    {
        if (entity)
            entity->SetTargetDirection(entity->GetPosition());
    }

    AIStateType GetType() const override { return stateType; }

    static const AIStateType stateType = AIStateType::Idle;

};
