#pragma once

#include "Entity.h"

#include <MathLib/math.h>
#include <memory>

class EntityFactory
{
	EntityFactory() { }
public:
	virtual ~EntityFactory() { }

	static std::shared_ptr<Entity> CreatePlayer(MathLib::vec2f & position);
	static std::shared_ptr<Entity> CreateEnemy(MathLib::vec2f & position);
	static std::shared_ptr<Entity> CreateProjectile(MathLib::vec2f & position, MathLib::vec2f & speed);

};