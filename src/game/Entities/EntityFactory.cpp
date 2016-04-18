#include <memory>

#include "Projectile.h"
#include "Enemy.h"
#include "EntityFactory.h"
#include "Player.h"

std::shared_ptr<Entity> EntityFactory::CreatePlayer()
{
	std::shared_ptr<Entity> entity(new Player);
	return entity;
}

std::shared_ptr<Entity> EntityFactory::CreateEnemy()
{
	std::shared_ptr<Entity> entity(new Enemy);
	return entity;
}

std::shared_ptr<Entity> EntityFactory::CreateProjectile()
{
	std::shared_ptr<Entity> entity(new Projectile);
	return entity;
}