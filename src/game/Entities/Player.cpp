#include "Player.h"
#include "Projectile.h"
#include "EntityFactory.h"

#include <cmath>

Player::Player(MathLib::vec2f & position) : Entity(position)
{
}

std::string Player::GetName()
{
    return "Player";
}

bool Player::handleEvent(const EntityEvent & event)
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

void Player::Shoot(MathLib::vec2f & targetPosition)
{
    MathLib::vec2f vec = targetPosition - position;
    // Decrease speed
    if (std::abs(vec.x) > std::abs(vec.y))
    {
        vec.y /= vec.x;
        vec.x /= vec.x;
    }
    else
    {
        vec.x /= vec.y;
        vec.y /= vec.y;
    }
    EntityFactory::CreateProjectile(position, vec);
}
