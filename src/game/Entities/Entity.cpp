#include "Entity.h"
#include "EntityManager.h"

Entity::Entity(MathLib::vec2f & position)
{
    this->position = position;
    subscribe(&EntityEventManager::GetInstance());
}

Entity::~Entity() {}

bool Entity::CalculateCollision(Entity & ent)
{
    auto isCollision = false;
    if (ent.GetPosition() == position) // TODO Add Range support
        isCollision = true;

    return isCollision;
}
