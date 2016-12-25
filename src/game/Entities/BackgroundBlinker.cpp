#include "BackgroundBlinker.h"

#include <stdexcept>

BackgroundBlinker::BackgroundBlinker(ShiftEngine::SpriteSceneNode *sprite)
    : Entity(sprite ? MathLib::vec2f{sprite->GetLocalPosition().x, sprite->GetLocalPosition().y} : MathLib::vec2f{}, sprite)
{
    blinkPeriod = 0.001f;
    blinkPhase = static_cast<float>(std::rand() % 628) / 100.0f;
}

void BackgroundBlinker::Update(double dt)
{
    time += dt;
    const float opacity = std::sinf((float)time * blinkPeriod + blinkPhase) / 2.0f * blinkOpacityInterval + blinkOpacityOffset;
    sprite->SetMaskColor({1.0f, 1.0f, 1.0f, opacity});
}

void BackgroundBlinker::SetBlinkPeriod(float period)
{
    blinkPeriod = period;
}
