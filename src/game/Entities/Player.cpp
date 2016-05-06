#include "Player.h"
#include "Projectile.h"
#include "EntityFactory.h"

#include <cmath>

Player::Player(MathLib::vec2f & position)
    : Entity(position)
{
}

bool Player::handleEvent(const ProjectilePositionEvent & event)
{
    if (CalculateCollision(*event.projectile))
        isToDelete = true;
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
