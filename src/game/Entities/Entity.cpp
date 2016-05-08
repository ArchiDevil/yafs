#include "Entity.h"
#include "EntityManager.h"

Entity::Entity(
    MathLib::vec2f & position,
    ShiftEngine::SpriteSceneNode * sprite)
        : position(position)
        , sprite(std::unique_ptr<ShiftEngine::SpriteSceneNode>(sprite))
{
    subscribe(&EntityEventManager::GetInstance());
}

Entity::~Entity()
{
    sprite->KillSelf();
}

bool Entity::CalculateCollision(const Entity & ent)
{
    auto isCollision = false;
    
    if (MathLib::isEqual(ent.GetPosition().x, position.x)
        && MathLib::isEqual(ent.GetPosition().y, position.y)) // TODO Add Range support
        isCollision = true;

    return isCollision;
}
