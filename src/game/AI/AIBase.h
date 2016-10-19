#pragma once

#include "Actions/AIAction.h"
#include <vector>
#include <memory>
#include "../Entities/LiveEntity.h"

class AIBase
{
protected:
    enum class AIState
    {
        Idle,
        Pursuit,
        Attack,
        Escape,
        Dodge,
        Death
    };

    AIState currentState = AIState::Idle;
    std::shared_ptr<AIAction> currentAction = nullptr;

public:
    AIBase() = default;
    virtual ~AIBase() = default;
    virtual void Update(double dt, LiveEntity * entity) { }
};
