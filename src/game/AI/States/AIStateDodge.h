#pragma once

#include "AIState.h"

class AIStateDodge final
    : public AIState
{
public:
    void Do(double dt, LiveEntity * entity) override;

};
