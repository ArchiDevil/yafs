#pragma once

#include "AIState.h"

class AIStateDodge final
    : public AIState
{
public:
    AIStateDodge(float dodgeDistance, float dodgeDistanceToProjectileLine);

    void Do(double dt, LiveEntity * entity) override;

    AIStateType GetType() const override { return stateType; }

    static const AIStateType stateType = AIStateType::Dodge;

private:
    float dodgeDistance;
    float dodgeDistanceToProjectileLine;
};
