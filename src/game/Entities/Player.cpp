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
    auto vec = MathLib::normalize<float>(targetPosition - position);

    EntityFactory::CreateProjectile(position, vec);
}
