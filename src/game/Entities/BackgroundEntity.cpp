#include "BackgroundEntity.h"

#include <stdexcept>

BackgroundEntity::BackgroundEntity(ShiftEngine::SpriteSceneNode *sprite, int layer)
    : Entity({0.0f, 0.0f})
    , layer(layer)
    , sprite(sprite)
{
    if (layer < 1 || layer > 3)
        throw std::runtime_error("layer must be > 0 and < 4");
}

void BackgroundEntity::Update(double dt)
{
    // Current behavior depends on layer where entity exists.
    // Of course, this should be in another class, but for now, 
    // it's ok to locate these things here.
    switch (layer)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    }
}

bool BackgroundEntity::handleEvent(const ProjectilePositionEvent & event)
{
    return false;
}
