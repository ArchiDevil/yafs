#pragma once

#include "Entity.h"

class EntityFactory
{
	EntityFactory() { }
public:
	virtual ~EntityFactory() { }

	static std::shared_ptr<Entity> CreatePlayer();
	static std::shared_ptr<Entity> CreateEnemy();
	static std::shared_ptr<Entity> CreateProjectile();
};