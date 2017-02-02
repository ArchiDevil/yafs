#pragma once

#include "AIBase.h"

#include <Utilities/logger.hpp>

class AISmallSpirit : public AIBase
{
public:
    AISmallSpirit(float lowHealthThresholdRatio);

    virtual ~AISmallSpirit() = default;
    void Update(double dt, LiveEntity * entity) override;

private:

#ifdef _DEBUG
    void printState(AIStateType stateType)
    {
        switch (stateType)
        {
        case AIStateType::Idle:
            LOG_INFO("Switching into waiting state");
            break;
        case AIStateType::Pursuit:
            LOG_INFO("Switching into pursuiting state");
            break;
        case AIStateType::Attack:
            LOG_INFO("Switching into attacking state");
            break;
        case AIStateType::Escape:
            LOG_INFO("Switching into escaping state");
            break;
        case AIStateType::Dodge:
            LOG_INFO("Switching into dodging state");
            break;
        case AIStateType::Death:
            LOG_INFO("Switching into dying state");
            break;
        default:
            LOG_INFO("Switching into unknown state");
            break;
        }
    }
#endif

    template<typename StateType>
    void setAction()
    {
        if (currentAction->GetType() != StateType::stateType)
        {
            currentAction = std::make_unique<StateType>();
#ifdef _DEBUG
            printState(StateType::stateType);
#endif
        }
    }

    template<typename StateType, typename ... Args>
    void setAction(Args && ...args)
    {
        if (currentAction->GetType() != StateType::stateType)
        {
            currentAction = std::make_unique<StateType>(std::forward<Args>(args)...);
#ifdef _DEBUG
            printState(StateType::stateType);
#endif
        }
    }

    float lowHealthThresholdRatio = 0.3f;
    bool isDying = false;

};
