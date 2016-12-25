#include "BackgroundWanderer.h"

constexpr float MOVE_EPS = 0.05f;

BackgroundWanderer::BackgroundWanderer(ShiftEngine::SpriteSceneNode *sprite)
    : BackgroundBlinker(sprite)
    , checkPeriod(static_cast<float>(rand() % 160 + 40) / 100.0f) // from 0.4 to 2.0 seconds
{
    BackgroundBlinker::blinkOpacityOffset = 0.8f;
    BackgroundBlinker::blinkOpacityInterval = 0.5f;
}

void BackgroundWanderer::Update(double dt)
{
    BackgroundBlinker::Update(dt);

    if (currentState == WandererState::Moving)
    {
        const float speed = 0.2f;
        Entity::position += MathLib::normalize(targetPosition - Entity::position) * speed * dt;

        // TODO: handle it automatically, this is ugly
        UpdateGraphicsSpritePosition();

        if (MathLib::distance(Entity::position, targetPosition) < MOVE_EPS)
        {
            currentState = WandererState::Waiting;
            targetPosition = {};
        }
    }

    if (BackgroundBlinker::time > lastCheckTime + static_cast<double>(checkPeriod))
    {
        lastCheckTime = BackgroundBlinker::time;
        if (currentState == WandererState::Moving)
            return;

        // let's check and get to new position
        const int chance = 5; // just 10% of chance to move
        const int checkedValue = rand() % 100;
        if (checkedValue < chance)
        {
            // set new target
            const float radius = (rand() % 100) * 2.0f / 100.0f + 1.0f;
            const float angle = (rand() % 100) * 6.28f / 100.0f;
            MathLib::vec2f vecToTransform = {1.0f, 0.0f};
            vecToTransform = MathLib::vec2Transform(vecToTransform, MathLib::matrixRotationZ(angle));
            vecToTransform *= radius;
            targetPosition = Entity::position + vecToTransform;
            currentState = WandererState::Moving;
        }
    }

}
