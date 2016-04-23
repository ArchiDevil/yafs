#include "Enemy.h"

Enemy::Enemy(MathLib::vec2f & position) : Entity(position)
{
}

std::string Enemy::GetName()
{
    return "Enemy";
}

bool Enemy::handleEvent(const EntityEvent & event)
{
    switch (event.type)
    {
    case EntityEventType::ProjectilePosition:
    {
        if (CalculateCollision(*static_cast<Entity *>(event.data)))
        {
            isToDelete = true;
        }
        break;
    }

    }

    return true;
}
