#pragma once

#include "AIState.h"

class AIStateDeath final
    : public AIState
{
public:
    void Do(double dt, LiveEntity * entity) override
    {
        dt; entity;
    }

    AIStateType GetType() const override { return stateType; }

    static const AIStateType stateType = AIStateType::Death;

};
