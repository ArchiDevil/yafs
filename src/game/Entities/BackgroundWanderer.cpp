#include "BackgroundWanderer.h"

constexpr float MOVE_EPS = 0.05f;

BackgroundWanderer::BackgroundWanderer(ShiftEngine::SpriteSceneNode *sprite)
    : BackgroundBlinker(sprite)
    , checkPeriod(static_cast<float>(rand() % 160 + 40) / 100.0f) // from 0.4 to 2.0 seconds
{
    BackgroundBlinker::blinkingOpacityOffset = 0.8f;
    BackgroundBlinker::blinkingOpacityInterval = 0.5f;
}

void BackgroundWanderer::Update(double dt)
{
    BackgroundBlinker::Update(dt);
    if (BackgroundBlinker::time > lastCheckTime + static_cast<double>(checkPeriod))
    {
        // let's check and get to new position
        int chance = 5; // just 10% of chance to move
        int checkedValue = rand() % 100;
        if (checkedValue < chance)
        {
            // set new target
            float radius = (rand() % 100) * 3.0f / 100.0f;
            float angle = (rand() % 100) * 6.28f / 100.0f;
            MathLib::vec2f vecToTransform = {1.0f, 0.0f};
            vecToTransform = MathLib::vec2Transform(vecToTransform, MathLib::matrixRotationZ(angle));
            vecToTransform *= radius;
            targetPosition = Entity::position + vecToTransform;
            currentState = WandererState::Moving;
        }
        lastCheckTime = BackgroundBlinker::time;
    }

    if (currentState == WandererState::Moving)
    {
        float speed = 0.05f;
        Entity::position += (targetPosition - Entity::position) * speed * dt;
        UpdateGraphicsSpritePosition();

        if (MathLib::distance(Entity::position, targetPosition) < MOVE_EPS)
        {
            currentState = WandererState::Waiting;
            targetPosition = {};
        }
    }
}
