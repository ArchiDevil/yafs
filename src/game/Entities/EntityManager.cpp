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

template<typename T>
void UpdateEntitiesList(std::vector<std::shared_ptr<T>>& from, std::vector<std::shared_ptr<T>>& to)
{
    if (from.size() > 0)
    {
        for (auto it = from.begin(); it != from.end(); ++it)
            to.push_back(*it);
        from.clear();
    }
}

template<typename T>
void ProcessEntityList(std::vector<std::shared_ptr<T>>& entitiesList, double dt)
{
    for (auto it = entitiesList.begin(); it != entitiesList.end(); ++it)
    {
        if (!(*it)->IsDead())
            (*it)->Update(dt);

        if ((*it)->IsDead())
        {
            if ((*it).use_count() == 1)
            {
                if (it + 1 == entitiesList.end())
                {
                    RemoveEntity(*it, entitiesList);
                    break;
                }

                RemoveEntity(*it, entitiesList);
                if (it != entitiesList.begin())
                    --it;
            }
        }
    }
}

void EntityManager::UpdateAllEntities(double dt)
{
    UpdateEntitiesList(entitiesToAdd, entities);
    UpdateEntitiesList(liveEntitiesToAdd, liveEntities);

    ProcessEntityList(entities, dt);
    ProcessEntityList(liveEntities, dt);
}

std::vector<std::shared_ptr<LiveEntity>> EntityManager::GetHostileLiveEntities(LiveEntity::Faction fraction)
{
    std::vector<std::shared_ptr<LiveEntity>> hostileEnt;

    for (auto it = liveEntities.begin(); it != liveEntities.end(); ++it)
    {
        if ((*it)->GetFaction() != fraction)
            hostileEnt.push_back(*it);
    }

    return hostileEnt;
}

std::shared_ptr<Player> EntityManager::CreatePlayer(const MathLib::vec2f & position,
                                                    float health)
{
    auto entity = factory.CreateEntity<Player>(position, health);
    AddEntity<LiveEntity>(entity, liveEntitiesToAdd);
    return entity;
}

std::shared_ptr<Enemy> EntityManager::CreateEnemy(const MathLib::vec2f & position,
                                                  float health,
                                                  int expCount,
                                                  Enemy::EnemyType type)
{
    std::unique_ptr<AIBase> ai;

    switch (type)
    {
    case Enemy::EnemyType::SmallSpirit:
        ai = std::make_unique<AISmallSpirit>(0.3f);
        break;
    default:
        assert(false);
        break;
    }

    auto entity = factory.CreateEntity<Enemy>(position, health, expCount, std::move(ai));
    AddEntity<LiveEntity>(entity, liveEntitiesToAdd);
    return entity;
}

std::shared_ptr<Projectile> EntityManager::CreateProjectile(const MathLib::vec2f & position,
                                                            const MathLib::vec2f & speed,
                                                            float damage,
                                                            double lifetime,
                                                            const LiveEntity* producer)
{
    auto entity = factory.CreateEntity<Projectile>(position, speed, damage, lifetime, producer);
    AddEntity<Entity>(entity, entitiesToAdd);
    return entity;
}

std::shared_ptr<ExperiencePoint> EntityManager::CreateExperiencePoint(const MathLib::vec2f & position,
                                                                      int expCount)
{
    auto entity = factory.CreateEntity<ExperiencePoint>(position, expCount);
    AddEntity<Entity>(entity, entitiesToAdd);
    return entity;
}

std::shared_ptr<MineDetectorEntity> EntityManager::CreateDetectorMine(const LiveEntity * owner,
                                                                      const MathLib::vec2f& position,
                                                                      float explosionDamage,
                                                                      float explosionRadius,
                                                                      float triggerDistance)
{
    auto entity = factory.CreateEntity<MineDetectorEntity>(owner, position, explosionDamage, explosionRadius, triggerDistance);
    AddEntity<Entity>(entity, entitiesToAdd);
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
    AddEntity<Entity>(entity, entitiesToAdd);
    return entity;
}

std::shared_ptr<BackgroundBlinker> EntityManager::CreateBackgroundBlinker(ShiftEngine::SpriteSceneNode * sprite)
{
    auto entity = factory.CreateEntity<BackgroundBlinker>(sprite);
    AddEntity<Entity>(entity, entitiesToAdd);
    return entity;
}

std::shared_ptr<BackgroundWanderer> EntityManager::CreateBackgroundWanderer(ShiftEngine::SpriteSceneNode * sprite)
{
    auto entity = factory.CreateEntity<BackgroundWanderer>(sprite);
    AddEntity<Entity>(entity, entitiesToAdd);
    return entity;
}

std::shared_ptr<VisualStickerEntity> EntityManager::CreateVisualStickerEntity(const LiveEntity * owner,
                                                                              const MathLib::vec2f& position,
                                                                              ShiftEngine::SpriteSceneNode * sprite)
{
    auto entity = factory.CreateEntity<VisualStickerEntity>(owner, position, sprite);
    AddEntity<Entity>(entity, entitiesToAdd);
    return entity;
}
