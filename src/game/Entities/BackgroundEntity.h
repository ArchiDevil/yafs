#pragma once

#include <GraphicsEngine/ShiftEngine.h>

#include "Entity.h"

// Just a background entity.
// Now it determines its behavior only on layer for example.
// In the future this class should has simple AI 
// declared somewhere in config file/script.
class BackgroundEntity
    : public Entity
{
public:
    BackgroundEntity(ShiftEngine::SpriteSceneNode *sprite, int layer);

    virtual void Update(double dt) override;
    virtual bool handleEvent(const ProjectilePositionEvent & event) override;

private:
    const int layer = 0;

};
