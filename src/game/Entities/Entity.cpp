#include "Entity.h"
#include "EntityManager.h"

Entity::Entity(const MathLib::vec2f & position,
               ShiftEngine::SpriteSceneNode * sprite)
    : position(position)
    , sprite(sprite)
{
    if (!sprite)
        throw std::runtime_error("sprite == nullptr");

    UpdateGraphicsSpritePosition();
}

Entity::~Entity()
{
}

bool Entity::handleEvent(const ProjectilePositionEvent& /*event*/)
{
    return true;
}

const MathLib::vec2f Entity::GetPosition() const
{
    return position;
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
    float radius = std::max({sprite->GetLocalScale().x, sprite->GetLocalScale().y, sprite->GetLocalScale().z});

    if (MathLib::distance(ent.GetPosition(), position) < radius) // then it is collided
        return true;

    return false;
}

void Entity::UpdateGraphicsSpritePosition()
{
    sprite->SetLocalPosition({position.x, position.y, 0.0});
}
