#pragma once

#include "AIState.h"

class AIStateEscape final
    : public AIState
{
public:
    void Do(double dt, LiveEntity * entity) override;

private:
    MathLib::vec2f runVector;

};
