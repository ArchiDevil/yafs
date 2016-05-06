#include "EntityManager.h"

#include <algorithm>

EntityManager::EntityManager()
{
    factory = std::make_shared<EntityFactory>();
}

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
    for (auto ent : entities )
    {
        ent->Update(dt);
        if (ent->IsToDelete())
        {
            RemoveEntity(ent);
        }
    }
}

std::shared_ptr<Player> EntityManager::CreatePlayer(MathLib::vec2f & position)
{
    auto entity = factory->CreatePlayer(position);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<Enemy> EntityManager::CreateEnemy(MathLib::vec2f & position)
{
    auto entity = factory->CreateEnemy(position);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<Projectile> EntityManager::CreateProjectile(MathLib::vec2f & position, MathLib::vec2f & speed)
{
    auto entity = factory->CreateProjectile(position, speed);
    AddEntity(entity);
    return entity;
}
