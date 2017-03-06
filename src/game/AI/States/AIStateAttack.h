#pragma once

#include "AIState.h"

class AIStateAttack final
    : public AIState
{
public:
    AIStateAttack(LiveEntity * entity);
    ~AIStateAttack();

    void Do(double dt, LiveEntity * entity) override;
    AIStateType GetType() const override { return stateType; }

    static const AIStateType stateType = AIStateType::Attack;

private:
    LiveEntity * entity = nullptr;

};
