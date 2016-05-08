#include "Projectile.h"
#include "EntityEventManager.h"

Projectile::Projectile(MathLib::vec2f & position, MathLib::vec2f & speed)
    : Entity(position)
    , speed(speed)
{
}

void Projectile::Update(double dt)
{
    position += speed * dt;

    EntityEventManager::GetInstance().notifyAll(ProjectilePositionEvent(this));
}
