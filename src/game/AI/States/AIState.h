#pragma once

#include "game\Entities\LiveEntity.h"

#include <memory>

class AIState
{
public:
    AIState() = default;
    virtual ~AIState() = default;
    virtual void Do(double dt, LiveEntity * entity) = 0;
};