#pragma once

#include "../../Entities/LiveEntity.h"

#include <memory>

class AIState
{
public:
    virtual ~AIState() = default;
    virtual void Do(double dt, LiveEntity * entity) = 0;

};
