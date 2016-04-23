#include "Projectile.h"
#include "EntityEventManager.h"

Projectile::Projectile(MathLib::vec2f & position, MathLib::vec2f & speed) : Entity(position)
{
    this->speed = speed;
}

std::string Projectile::GetName()
{
    return "Projectile";
}

void Projectile::Update(double deltaTime)
{
    position += speed * deltaTime;

    EntityEventManager::GetInstance().notifyAll(EntityEvent(EntityEventType::ProjectilePosition, this));
}
