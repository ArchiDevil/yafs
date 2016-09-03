#include "EntityManager.h"

#include <algorithm>

using namespace MathLib;

void EntityManager::AddEntity(const std::shared_ptr<Entity> & ent)
{
    entities.push_back(ent);
}

void EntityManager::UpdateAllEntities(double dt)
{
    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        (*it)->Update(dt);
        if ((*it)->IsDead())
        {
            if (it + 1 == entities.end())
                return RemoveEntity(*it);

            RemoveEntity(*it);
            if (it != entities.begin())
                --it;
        }
    }
}

std::shared_ptr<Player> EntityManager::CreatePlayer(const MathLib::vec2f & position,
                                                    float health)
{
    auto entity = factory->CreateEntity<Player>(position, health);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<Enemy> EntityManager::CreateEnemy(const MathLib::vec2f & position,
                                                  float health)
{
    auto entity = factory->CreateEntity<Enemy>(position, health);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<Projectile> EntityManager::CreateProjectile(const MathLib::vec2f & position,
                                                            const MathLib::vec2f & speed,
                                                            float damage,
                                                            double lifetime,
                                                            Entity* producer)
{
    auto entity = factory->CreateEntity<Projectile>(position, speed, damage, lifetime, producer);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<BackgroundBlinker> EntityManager::CreateBackgroundBlinker(ShiftEngine::SpriteSceneNode *sprite)
{
    auto entity = factory->CreateEntity<BackgroundBlinker>(sprite);
    AddEntity(entity);
    return entity;
}

void EntityManager::RemoveEntity(std::shared_ptr<Entity> & ent)
{
    std::swap(ent, entities.back());
    entities.pop_back();
}
