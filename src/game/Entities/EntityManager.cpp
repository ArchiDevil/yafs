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

std::shared_ptr<Player> EntityManager::CreatePlayer(const vec2f & position)
{
    auto entity = factory->CreateEntity<Player>(position);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<Enemy> EntityManager::CreateEnemy(const vec2f & position)
{
    auto entity = factory->CreateEntity<Enemy>(position);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<Projectile> EntityManager::CreateProjectile(const vec2f & position, const vec2f & speed, Entity* producer)
{
    auto entity = factory->CreateEntity<Projectile>(position, speed, producer);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<BackgroundEntity> EntityManager::CreateBackgroundEntity(ShiftEngine::SpriteSceneNode *sprite, int layer)
{
    auto entity = factory->CreateEntity<BackgroundEntity>(sprite, layer);
    AddEntity(entity);
    return entity;
}

void EntityManager::RemoveEntity(std::shared_ptr<Entity> & ent)
{
    std::swap(ent, entities.back());
    entities.pop_back();
}
