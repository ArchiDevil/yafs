#include "EntityFactory.h"
#include "EntityManager.h"

std::shared_ptr<Player> EntityFactory::CreatePlayer(MathLib::vec2f & position)
{
    std::shared_ptr<Player> entity(new Player(position));
    EntityManager::GetInstance().AddEntity(entity);
    return entity;
}

std::shared_ptr<Enemy> EntityFactory::CreateEnemy(MathLib::vec2f & position)
{
    std::shared_ptr<Enemy> entity(new Enemy(position));
    EntityManager::GetInstance().AddEntity(entity);
    return entity;
}

std::shared_ptr<Projectile> EntityFactory::CreateProjectile(MathLib::vec2f & position, MathLib::vec2f & speed)
{
    std::shared_ptr<Projectile> entity(new Projectile(position, speed));
    EntityManager::GetInstance().AddEntity(entity);
    return entity;
}