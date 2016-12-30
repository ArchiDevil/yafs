#pragma once
#include "AIState.h"


class AIStateEscape : public AIState
{
public:
    virtual ~AIStateEscape() = default;

    void Do(double dt, LiveEntity * entity) override;

private:
    MathLib::vec2f runVector;

};
