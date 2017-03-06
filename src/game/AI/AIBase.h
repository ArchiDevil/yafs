#pragma once

#include "States/AIState.h"
#include "../Entities/LiveEntity.h"

#include <memory>

class AIBase
{
public:
    virtual ~AIBase() = default;
    virtual void Update(double dt, LiveEntity * entity) { dt; entity; }

protected:
    std::unique_ptr<AIState> currentAction;

};
