#pragma once

#include "Enemy.h"
#include "Player.h"
#include "Projectile.h"

#include <MathLib/math.h>
#include <memory>

class EntityFactory
{
	EntityFactory() { }
public:
	virtual ~EntityFactory() { }

	static std::shared_ptr<Player> CreatePlayer(MathLib::vec2f & position);
	static std::shared_ptr<Enemy> CreateEnemy(MathLib::vec2f & position);
	static std::shared_ptr<Projectile> CreateProjectile(MathLib::vec2f & position, MathLib::vec2f & speed);

};