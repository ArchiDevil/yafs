#pragma once

#include "../../Entities/LiveEntity.h"

enum class AIStateType
{
    Idle,
    Pursuit,
    Attack,
    Escape,
    Dodge,
    Death
};

class AIState
{
public:
    virtual ~AIState() = default;
    virtual void Do(double dt, LiveEntity * entity) = 0;
    virtual AIStateType GetType() const = 0;

};
