#include "EntityManager.h"

#include <algorithm>

using namespace MathLib;

template<typename T1, typename T2>
void EntityManager::AddEntity(const std::shared_ptr<T1> & ent, std::vector<std::shared_ptr<T2>> list)
{
    list.push_back(ent);
}

void EntityManager::UpdateAllEntities(double dt)
{
    if (entitiesToAdd.size() > 0)
    {
        for (auto it = entitiesToAdd.begin(); it != entitiesToAdd.end(); ++it)
            entities.push_back(*it);
        entitiesToAdd.clear();
    }
    if (liveEntitiesToAdd.size() > 0)
    {
        for (auto it = liveEntitiesToAdd.begin(); it != liveEntitiesToAdd.end(); ++it)
            liveEntities.push_back(*it);
        liveEntitiesToAdd.clear();
    }

    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        if (!(*it)->IsDead())
            (*it)->Update(dt);

        if ((*it)->IsDead())
        {
            if ((*it).use_count() == 1)
            {
                if (it + 1 == entities.end())
                    return RemoveEntity(*it, entities);

                RemoveEntity(*it, entities);
                if (it != entities.begin())
                    --it;
            }
        }
    }

    for (auto it = liveEntities.begin(); it != liveEntities.end(); ++it)
    {
        if (!(*it)->IsDead())
            (*it)->Update(dt);

        if ((*it)->IsDead())
        {
            if ((*it).use_count() == 1)
            {
                if (it + 1 == liveEntities.end())
                    return RemoveEntity(*it, liveEntities);

                RemoveEntity(*it, liveEntities);
                if (it != liveEntities.begin())
                    --it;
            }
        }
    }
}

std::shared_ptr<Player> EntityManager::CreatePlayer(const MathLib::vec2f & position,
                                                    float health)
{
    auto entity = factory->CreateEntity<Player>(position, health);
    AddEntity(entity, liveEntities);
    return entity;
}

std::shared_ptr<Enemy> EntityManager::CreateEnemy(const MathLib::vec2f & position,
                                                  float health,
                                                  int expCount)
{
    auto entity = factory->CreateEntity<Enemy>(position, health, expCount);
    AddEntity(entity, liveEntities);
    return entity;
}

std::shared_ptr<Projectile> EntityManager::CreateProjectile(const MathLib::vec2f & position,
                                                            const MathLib::vec2f & speed,
                                                            float damage,
                                                            double lifetime,
                                                            Entity* producer)
{
    auto entity = factory->CreateEntity<Projectile>(position, speed, damage, lifetime, producer);
    AddEntity(entity, entities);
    return entity;
}

std::shared_ptr<BackgroundBlinker> EntityManager::CreateBackgroundBlinker(ShiftEngine::SpriteSceneNode *sprite)
{
    auto entity = factory->CreateEntity<BackgroundBlinker>(sprite);
    AddEntity(entity, entities);
    return entity;
}

std::shared_ptr<BackgroundWanderer> EntityManager::CreateBackgroundWanderer(ShiftEngine::SpriteSceneNode *sprite)
{
    auto entity = factory->CreateEntity<BackgroundWanderer>(sprite);
    AddEntity(entity, entities);
    return entity;
}

std::shared_ptr<ExperiencePoint> EntityManager::CreateExperiencePoint(const MathLib::vec2f & position, int expCount)
{
    auto entity = factory->CreateEntity<ExperiencePoint>(position, expCount);
    AddEntity(entity, entities);
    return entity;
}

template<typename T>
void EntityManager::RemoveEntity(std::shared_ptr<T> & ent, std::vector<std::shared_ptr<T>> list)
{
    std::swap(ent, list.back());
    list.pop_back();
}

const std::vector<std::shared_ptr<LiveEntity>> * EntityManager::GetLiveEntities()
{
    return &liveEntities;
}
