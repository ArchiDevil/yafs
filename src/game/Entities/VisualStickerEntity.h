#pragma once

#include "VisualEntity.h"
#include "LiveEntity.h"

class VisualStickerEntity
    : public VisualEntity
{
public:
    // this requires LiveEntity instead of VisualEntity due to unfinished moving 
    // all entites hierarchy to inherit all visual entities from VisualEntity
    VisualStickerEntity(const LiveEntity * owner, const MathLib::vec2f & position, ShiftEngine::SpriteSceneNode * sprite)
        : VisualEntity(position, sprite)
        , owner(owner)
    {
    }

    void Update(double /*dt*/) override
    {
        if (owner)
        {
            Entity::position = owner->GetPosition();
            Entity::UpdateGraphicsSpritePosition();
        }
    }

private:
    const LiveEntity * owner = nullptr;

};
