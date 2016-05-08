#include "Enemy.h"
#include "Projectile.h"

Enemy::Enemy(MathLib::vec2f & position)
    : Entity(position)
{
}

bool Enemy::handleEvent(const ProjectilePositionEvent & event)
{
    if (CalculateCollision(*event.projectile))
        isToDelete = true;
    return true;
}
