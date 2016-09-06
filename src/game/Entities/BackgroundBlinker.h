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

    void SetBlinkPeriod(float period);

protected:
    float blinkPeriod = 0.0f;
    float blinkPhase = 0.0f;
    float blinkOpacityInterval = 0.3f;
    float blinkOpacityOffset = 0.05f;
    double time = 0.0;

};
