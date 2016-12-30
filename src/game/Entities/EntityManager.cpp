#include "EntityManager.h"

#include "..\AI\AISmallSpirit.h"

#include <algorithm>

using namespace MathLib;

template<typename T>
void AddEntity(const std::shared_ptr<T> & ent, std::vector<std::shared_ptr<T>> & list)
{
    list.push_back(ent);
}

template<typename T>
void RemoveEntity(std::shared_ptr<T> & ent, std::vector<std::shared_ptr<T>> & list)
{
    std::swap(ent, list.back());
    list.pop_back();
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
                {
                    RemoveEntity(*it, entities);
                    break;
                }

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
                {
                    RemoveEntity(*it, liveEntities);
                    break;
                }

                RemoveEntity(*it, liveEntities);
                if (it != liveEntities.begin())
                    --it;
            }
        }
    }
}

const std::vector<std::shared_ptr<LiveEntity>> * EntityManager::GetLiveEntities()
{
    return &liveEntities;
}

const std::vector<std::shared_ptr<LiveEntity>> * EntityManager::GetHostileLiveEntities(LiveEntity::Faction fraction)
{
    std::vector<std::shared_ptr<LiveEntity>> * hostileEnt = new std::vector<std::shared_ptr<LiveEntity>>();

    for (auto it = liveEntities.begin(); it != liveEntities.end(); ++it)
    {
        if ((*it)->GetFaction() != fraction)
            hostileEnt->push_back(*it);
    }

    return hostileEnt;
}

std::shared_ptr<Player> EntityManager::CreatePlayer(const MathLib::vec2f & position,
                                                    float health)
{
    auto entity = factory.CreateEntity<Player>(position, health);
    AddEntity((std::shared_ptr<LiveEntity>)entity, liveEntitiesToAdd);
    return entity;
}

std::shared_ptr<Enemy> EntityManager::CreateEnemy(const MathLib::vec2f & position,
                                                  float health,
                                                  int expCount,
                                                  Enemy::EnemyType type)
{
    std::shared_ptr<AIBase> ai;

    switch (type)
    {
    case Enemy::EnemyType::SmallSpirit:
        ai = std::make_shared<AISmallSpirit>();
        break;
    default:
        assert(false);
        break;
    }

    auto entity = factory.CreateEntity<Enemy>(position, health, expCount, ai);
    AddEntity((std::shared_ptr<LiveEntity>)entity, liveEntitiesToAdd);
    return entity;
}

std::shared_ptr<Projectile> EntityManager::CreateProjectile(const MathLib::vec2f & position,
                                                            const MathLib::vec2f & speed,
                                                            float damage,
                                                            double lifetime,
                                                            const LiveEntity* producer)
{
    auto entity = factory.CreateEntity<Projectile>(position, speed, damage, lifetime, producer);
    AddEntity((std::shared_ptr<Entity>)entity, entitiesToAdd);
    return entity;
}

std::shared_ptr<ExperiencePoint> EntityManager::CreateExperiencePoint(const MathLib::vec2f & position,
                                                                      int expCount)
{
    auto entity = factory.CreateEntity<ExperiencePoint>(position, expCount);
    AddEntity((std::shared_ptr<Entity>)entity, entitiesToAdd);
    return entity;
}

std::shared_ptr<MineDetectorEntity> EntityManager::CreateDetectorMine(const LiveEntity * owner,
                                                                      const MathLib::vec2f& position,
                                                                      float explosionDamage,
                                                                      float explosionRadius,
                                                                      float triggerDistance)
{
    auto entity = factory.CreateEntity<MineDetectorEntity>(owner, position, explosionDamage, explosionRadius, triggerDistance);
    AddEntity((std::shared_ptr<Entity>)entity, entitiesToAdd);
    return entity;
}

std::shared_ptr<MineTimedEntity> EntityManager::CreateTimedMine(const LiveEntity * owner,
                                                                const MathLib::vec2f& position,
                                                                float explosionDamage,
                                                                float explosionRadius,
                                                                float triggerDistance,
                                                                double timeToExplode)
{
    auto entity = factory.CreateEntity<MineTimedEntity>(owner, position, explosionDamage, explosionRadius, triggerDistance, timeToExplode);
    AddEntity((std::shared_ptr<Entity>)entity, entitiesToAdd);
    return entity;
}

std::shared_ptr<BackgroundBlinker> EntityManager::CreateBackgroundBlinker(ShiftEngine::SpriteSceneNode * sprite)
{
    auto entity = factory.CreateEntity<BackgroundBlinker>(sprite);
    AddEntity((std::shared_ptr<Entity>)entity, entitiesToAdd);
    return entity;
}

std::shared_ptr<BackgroundWanderer> EntityManager::CreateBackgroundWanderer(ShiftEngine::SpriteSceneNode * sprite)
{
    auto entity = factory.CreateEntity<BackgroundWanderer>(sprite);
    AddEntity((std::shared_ptr<Entity>)entity, entitiesToAdd);
    return entity;
}

std::shared_ptr<VisualStickerEntity> EntityManager::CreateVisualStickerEntity(const LiveEntity * owner,
                                                                              const MathLib::vec2f& position,
                                                                              ShiftEngine::SpriteSceneNode * sprite)
{
    auto entity = factory.CreateEntity<VisualStickerEntity>(owner, position, sprite);
    AddEntity((std::shared_ptr<Entity>)entity, entitiesToAdd);
    return entity;
}
