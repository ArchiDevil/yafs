#include "EntityFactory.h"
#include "EntityManager.h"

std::shared_ptr<Player> EntityFactory::CreatePlayer(const MathLib::vec2f & position) const
{
    return std::make_shared<Player>(position);
}

std::shared_ptr<Enemy> EntityFactory::CreateEnemy(const MathLib::vec2f & position) const
{
    return std::make_shared<Enemy>(position);
}

std::shared_ptr<Projectile> EntityFactory::CreateProjectile(const MathLib::vec2f & position, const MathLib::vec2f & speed) const
{
    return std::make_shared<Projectile>(position, speed);
}
