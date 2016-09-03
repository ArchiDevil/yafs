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

protected:
    float blinkingPeriod = 0.0f;
    float blinkingPhase = 0.0f;
    float blinkingOpacityInterval = 0.3f;
    float blinkingOpacityOffset = 0.05f;
    double time = 0.0;

};
