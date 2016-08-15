#include "Entity.h"
#include "EntityManager.h"

Entity::Entity(const MathLib::vec2f & position,
               ShiftEngine::SpriteSceneNode * sprite)
    : position(position)
    , sprite(sprite)
{
    UpdateGraphicsSpritePosition();
}

Entity::~Entity()
{
}

void Entity::Show()
{
}

void Entity::Hide()
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
    auto isCollision = false;

    if (MathLib::isEqual(ent.GetPosition().x, position.x) &&
        MathLib::isEqual(ent.GetPosition().y, position.y)) // TODO Add Range support
        isCollision = true;

    return isCollision;
}

void Entity::UpdateGraphicsSpritePosition()
{
    if (sprite)
        sprite->SetLocalPosition({position.x, position.y, 0.0});
}
