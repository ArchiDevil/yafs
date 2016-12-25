#pragma once

#include "States/AIState.h"
#include <vector>
#include <memory>
#include "../Entities/LiveEntity.h"

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
protected:


    AIStateType currentState = AIStateType::Idle;
    std::shared_ptr<AIState> currentAction = nullptr;

public:
    AIBase() = default;
    virtual ~AIBase() = default;
    virtual void Update(double dt, LiveEntity * entity) { }
};
