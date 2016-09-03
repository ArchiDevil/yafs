#include "BackgroundBlinker.h"

#include <stdexcept>

BackgroundBlinker::BackgroundBlinker(ShiftEngine::SpriteSceneNode *sprite)
    : Entity(sprite ? MathLib::vec2f{sprite->GetLocalPosition().x, sprite->GetLocalPosition().y} : MathLib::vec2f{}, sprite)
{
    blinkingPeriod = 0.001f;
    blinkingPhase = static_cast<float>(std::rand() % 628) / 100.0f;
}

void BackgroundBlinker::Update(double dt)
{
    time += dt;
    float opacity = std::sinf((float)time * blinkingPeriod + blinkingPhase) / 2.0f * blinkingOpacityInterval + blinkingOpacityOffset;
    sprite->SetMaskColor({1.0f, 1.0f, 1.0f, opacity});
}
