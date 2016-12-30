#pragma once

#include "States/AIState.h"
#include "../Entities/LiveEntity.h"

#include <vector>
#include <memory>

enum class AIStateType
{
    Idle,
    Pursuit,
    Attack,
    Escape,
    Dodge,
    Death
};

class AIBase
{
public:
    virtual ~AIBase() = default;
    virtual void Update(double dt, LiveEntity * entity) { }

protected:
    AIStateType currentState = AIStateType::Idle;
    std::shared_ptr<AIState> currentAction = nullptr;

};
