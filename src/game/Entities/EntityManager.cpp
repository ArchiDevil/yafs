#include "EntityManager.h"

#include <algorithm>

using namespace MathLib;

void EntityManager::AddEntity(const std::shared_ptr<Entity> & ent)
{
    entities.push_back(ent);
}

void EntityManager::RemoveEntity(const std::shared_ptr<Entity> & ent)
{
    auto count = entities.size();
    for (size_t i = 0; i < count; ++i)
    {
        if (entities[i] == ent)
        {
            std::swap(entities[i], entities.back());
            entities.pop_back();
            break;
        }
    }
}

void EntityManager::UpdateAllEntities(double dt)
{
    for (auto &ent : entities)
    {
        ent->Update(dt);
        if (ent->IsToDelete())
        {
            RemoveEntity(ent);
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

std::shared_ptr<Projectile> EntityManager::CreateProjectile(const vec2f & position, const vec2f & speed)
{
    auto entity = factory->CreateEntity<Projectile>(position, speed);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<BackgroundEntity> EntityManager::CreateBackgroundEntity(ShiftEngine::SpriteSceneNode *sprite, int layer)
{
    auto entity = factory->CreateEntity<BackgroundEntity>(sprite, layer);
    AddEntity(entity);
    return entity;
}
