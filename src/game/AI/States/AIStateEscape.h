#pragma once
#include "AIState.h"


class AIStateEscape : public AIState
{
public:
    AIStateEscape();
    virtual ~AIStateEscape() = default;
    void Do(double dt, LiveEntity * entity) override;

private:
    MathLib::vec2f runVector = { 0.0f, 0.0f };
};
