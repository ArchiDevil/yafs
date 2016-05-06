#include "EntityFactory.h"
#include "EntityManager.h"

std::shared_ptr<Player> EntityFactory::CreatePlayer(MathLib::vec2f & position)
{
    return std::make_shared<Player>(position);
}

std::shared_ptr<Enemy> EntityFactory::CreateEnemy(MathLib::vec2f & position)
{
    return std::make_shared<Enemy>(position);
}

std::shared_ptr<Projectile> EntityFactory::CreateProjectile(MathLib::vec2f & position, MathLib::vec2f & speed)
{
    return std::make_shared<Projectile>(position, speed);
}