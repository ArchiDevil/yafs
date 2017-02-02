#pragma once

#include "AIState.h"

class AIStatePursuit final
    : public AIState
{
public:
    void Do(double dt, LiveEntity * entity) override;

    AIStateType GetType() const override { return stateType; }

    static const AIStateType stateType = AIStateType::Pursuit;

};
