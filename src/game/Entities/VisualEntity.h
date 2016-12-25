#pragma once

#include "Entity.h"

// For now, this is just proxy to entity without anything
// In future, I plan to remove visual part from entity and move it here
// Because some entities has no visual part (invisible walls, sound, trigger areas, etc.)
class VisualEntity
    : public Entity
{
public:
    VisualEntity(const MathLib::vec2f & position, ShiftEngine::SpriteSceneNode * sprite)
        : Entity(position, sprite)
    {
    }
};
