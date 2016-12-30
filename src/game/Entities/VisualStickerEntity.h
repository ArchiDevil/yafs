#pragma once

#include "VisualEntity.h"
#include "LiveEntity.h"

class VisualStickerEntity final
    : public VisualEntity
{
public:
    // this requires LiveEntity instead of VisualEntity due to unfinished moving 
    // all entities hierarchy to inherit all visual entities from VisualEntity
    VisualStickerEntity(const LiveEntity * owner, MathLib::vec2f position, ShiftEngine::SpriteSceneNode * sprite)
        : VisualEntity(position, sprite)
        , owner(owner)
    {
    }

    void Update(double /*dt*/) override
    {
        if (owner)
            Entity::SetPosition(owner->GetPosition());
    }

private:
    const LiveEntity * owner = nullptr;

};
