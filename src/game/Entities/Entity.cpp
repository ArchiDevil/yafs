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

const MathLib::vec2f Entity::GetPosition() const
{
    return position;
}

void Entity::SetPosition(MathLib::vec2f & pos)
{
    position = pos;
}

bool Entity::IsDead() const
{
    return isToDelete;
}

void Entity::Kill()
{
    isToDelete = true;
}

bool Entity::CalculateCollision(const Entity & ent) const
{
    const float radius = std::max({sprite->GetLocalScale().x, sprite->GetLocalScale().y, sprite->GetLocalScale().z});

    if (MathLib::distance(ent.GetPosition(), position) < radius) // then it is collided
        return true;

    return false;
}

void Entity::UpdateGraphicsSpritePosition()
{
    sprite->SetLocalPosition({position.x, position.y, 0.0});
}
