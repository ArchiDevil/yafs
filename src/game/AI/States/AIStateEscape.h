#pragma once

#include "AIState.h"

class AIStateEscape final
    : public AIState
{
public:
    AIStateEscape(float escapeDistance);

    void Do(double dt, LiveEntity * entity) override;
    AIStateType GetType() const override { return stateType; }

    static const AIStateType stateType = AIStateType::Escape;

private:
    const float escapeDistance;
    MathLib::vec2f runVector;

};
