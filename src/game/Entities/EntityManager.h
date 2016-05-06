#pragma once

#include "Entity.h"
#include "EntityFactory.h"

#include <memory>
#include <Utilities/singleton.h>
#include <vector>

class EntityManager
{
public:
    EntityManager();
    virtual ~EntityManager() {}

    void AddEntity(const std::shared_ptr<Entity> & ent);
    void RemoveEntity(const std::shared_ptr<Entity> & ent);

    void UpdateAllEntities(double dt);

    std::shared_ptr<Player> CreatePlayer(MathLib::vec2f & position);
    std::shared_ptr<Enemy> CreateEnemy(MathLib::vec2f & position);
    std::shared_ptr<Projectile> CreateProjectile(MathLib::vec2f & position, MathLib::vec2f & speed);

private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::shared_ptr<EntityFactory> factory;
};