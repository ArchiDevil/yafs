#pragma once

#include "BackgroundBlinker.h"

// Entity, that is used on background
// Sometimes it starts it's moving to some point near it's position
// Then it stops and waits until other movement
class BackgroundWanderer final
    : public BackgroundBlinker
{
    enum class WandererState
    {
        Moving,
        Waiting
    };

public:
    BackgroundWanderer(ShiftEngine::SpriteSceneNode *sprite);
    virtual void Update(double dt) override;

protected:
    // this value defines how fast this entity checks it's wish for movement
    float checkPeriod = 0.0;

    // last time when entity checked it's wish
    double lastCheckTime = 0.0;

    // target to move to
    MathLib::vec2f targetPosition = {};

    // current entity state
    WandererState currentState = WandererState::Waiting;

};
