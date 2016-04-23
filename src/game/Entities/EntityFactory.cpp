#include "EntityFactory.h"

#include "EntityManager.h"
#include "Enemy.h"
#include "Player.h"
#include "Projectile.h"

std::shared_ptr<Entity> EntityFactory::CreatePlayer(MathLib::vec2f & position)
{
    std::shared_ptr<Entity> entity(new Player(position));
    EntityManager::GetInstance().AddEnity(entity);
    return entity;
}

std::shared_ptr<Entity> EntityFactory::CreateEnemy(MathLib::vec2f & position)
{
    std::shared_ptr<Entity> entity(new Enemy(position));
    EntityManager::GetInstance().AddEnity(entity);
    return entity;
}

std::shared_ptr<Entity> EntityFactory::CreateProjectile(MathLib::vec2f & position, MathLib::vec2f & speed)
{
    std::shared_ptr<Entity> entity(new Projectile(position, speed));
    EntityManager::GetInstance().AddEnity(entity);
    return entity;
}