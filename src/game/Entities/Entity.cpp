#include "Entity.h"
#include "EntityManager.h"

Entity::Entity(MathLib::vec2f & position)
{
    this->position = position;
    subscribe(&EntityEventManager::GetInstance());
}

Entity::~Entity() {}

bool Entity::CalculateCollision(const Entity & ent)
{
    auto isCollision = false;
    
    if (MathLib::isEqual(ent.GetPosition().x, position.x)
        && MathLib::isEqual(ent.GetPosition().y, position.y)) // TODO Add Range support
        isCollision = true;

    return isCollision;
}
