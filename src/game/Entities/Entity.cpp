#include "Entity.h"
#include "EntityManager.h"

Entity::Entity(const MathLib::vec2f & position,
               ShiftEngine::SpriteSceneNode * sprite)
    : position(position)
    , sprite(sprite)
{
    SetSpritePosition();
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

void Entity::Move(double x, double y)
{
    x, y;
}

bool Entity::handleEvent(const ProjectilePositionEvent& /*event*/)
{
    return true;
}

const MathLib::vec2f Entity::GetPosition() const
{
    return position;
}

bool Entity::IsDead()
{
    return isToDelete;
}

void Entity::Die()
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

void Entity::SetSpritePosition()
{
    if (sprite)
        sprite->SetLocalPosition(MathLib::vec3f(position.x, position.y, 0.0));
}
