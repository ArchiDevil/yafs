#include "EntityManager.h"

#include <algorithm>

using namespace MathLib;

void EntityManager::AddEntity(const std::shared_ptr<Entity> & ent)
{
    entitiesToAdd.push_back(ent);
}

void EntityManager::UpdateAllEntities(double dt)
{
    if (entitiesToAdd.size() > 0)
    {
        for (auto it = entitiesToAdd.begin(); it != entitiesToAdd.end(); ++it)
        {
            entities.push_back(*it);
            if (dynamic_cast<LiveEntity *>(it->get()))
                liveEntities.push_back(*it);
        }
        entitiesToAdd.clear();
    }

    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        if (!(*it)->IsDead())
            (*it)->Update(dt);

        if ((*it)->IsDead())
        {
            if ((*it).use_count() == 1 ||
                (dynamic_cast<LiveEntity *>(it->get()) && (*it).use_count() == 2))
            {
                if (it + 1 == entities.end())
                    return RemoveEntity(*it);

                RemoveEntity(*it);
                if (it != entities.begin())
                    --it;
            }
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
                                                  float health,
                                                  int expCount)
{
    auto entity = factory->CreateEntity<Enemy>(position, health, expCount);
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

std::shared_ptr<BackgroundWanderer> EntityManager::CreateBackgroundWanderer(ShiftEngine::SpriteSceneNode *sprite)
{
    auto entity = factory->CreateEntity<BackgroundWanderer>(sprite);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<ExperiencePoint> EntityManager::CreateExperiencePoint(const MathLib::vec2f & position, int expCount)
{
    auto entity = factory->CreateEntity<ExperiencePoint>(position, expCount);
    AddEntity(entity);
    return entity;
}

void EntityManager::RemoveEntity(std::shared_ptr<Entity> & ent)
{

    if (dynamic_cast<LiveEntity *>(ent.get()))
    {
        std::swap(ent, liveEntities.back());
        liveEntities.pop_back();
    }

    std::swap(ent, entities.back());
    entities.pop_back();
}

std::vector<std::shared_ptr<Entity>> EntityManager::GetLiveEntities()
{
    return liveEntities;
}
