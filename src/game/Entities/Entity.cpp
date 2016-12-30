#include "Entity.h"
#include "EntityManager.h"

#include <string>

Entity::Entity(const MathLib::vec2f & position,
               ShiftEngine::SpriteSceneNode * sprite)
    : position(position)
    , sprite(sprite)
{
    if (!sprite)
        throw std::runtime_error("sprite == nullptr");

    sprite->SetDrawingMode(ShiftEngine::SpriteSceneNode::SpriteDrawingMode::Additive);

    UpdateGraphicsSpritePosition();
}

MathLib::vec2f Entity::GetPosition() const
{
    return position;
}

void Entity::SetPosition(MathLib::vec2f pos)
{
    this->position = pos;
    UpdateGraphicsSpritePosition();
}

bool Entity::IsDead() const
{
    return isToDelete;
}

void Entity::Kill()
{
    isToDelete = true;
    sprite.reset();
}

void Entity::UpdateGraphicsSpritePosition()
{
    sprite->SetLocalPosition({position.x, position.y, 0.0});
}
