#include "EntityManager.h"

void EntityManager::AddEnity(std::shared_ptr<Entity> ent)
{
    entities.push_back(ent);
}

void EntityManager::RemoveEntity(std::shared_ptr<Entity> ent)
{
    entities.remove(ent);
}

void EntityManager::UpdateAllEntities(double deltaTime)
{
    for (auto ent : entities )
    {
        ent->Update(deltaTime);
        if (ent->IsToDelete())
        {
            RemoveEntity(ent);
        }
    }
}
