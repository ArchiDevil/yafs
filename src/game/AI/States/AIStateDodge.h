#pragma once

#include "AIState.h"

class AIStateDodge final
    : public AIState
{
public:
    void Do(double dt, LiveEntity * entity) override
    {
        dt; entity;
    }

    AIStateType GetType() const override { return stateType; }

    static const AIStateType stateType = AIStateType::Dodge;

};
