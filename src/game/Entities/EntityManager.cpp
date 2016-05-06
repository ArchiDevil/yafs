#include "EntityManager.h"

#include <algorithm>

void EntityManager::AddEntity(const std::shared_ptr<Entity> & ent)
{
    entities.push_back(ent);
}

void EntityManager::RemoveEntity(const std::shared_ptr<Entity> & ent)
{
    auto count = entities.size();
    for (auto i = 0; i < count; ++i)
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
