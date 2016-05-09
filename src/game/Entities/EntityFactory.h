#pragma once

#include "Enemy.h"
#include "Player.h"
#include "Projectile.h"

#include <MathLib/math.h>
#include <memory>

class EntityFactory
{
public:
    EntityFactory() { }
    virtual ~EntityFactory() { }

    std::shared_ptr<Player> CreatePlayer(const MathLib::vec2f & position) const;
    std::shared_ptr<Enemy> CreateEnemy(const MathLib::vec2f & position) const;
    std::shared_ptr<Projectile> CreateProjectile(const MathLib::vec2f & position, const MathLib::vec2f & speed) const;

};
