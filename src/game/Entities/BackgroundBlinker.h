#pragma once

#include <GraphicsEngine/ShiftEngine.h>

#include "Entity.h"

// Just a background blinker.
// Just blinks somewhere on the background.
// Slowly fades in and out.
class BackgroundBlinker
    : public Entity
{
public:
    BackgroundBlinker(ShiftEngine::SpriteSceneNode *sprite);

    virtual void Update(double dt) override;
    virtual bool handleEvent(const ProjectilePositionEvent & event) override;

private:
    float blinkingPeriod = 0.0f;
    float blinkingPhase = 0.0f;

};
