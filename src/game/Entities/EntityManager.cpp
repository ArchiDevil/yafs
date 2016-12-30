#include "EntityManager.h"

#include <algorithm>

using namespace MathLib;

EntityManager::EntityManager(Physics::PhysicsManager* physicsMgr)
    : physicsMgr(physicsMgr)
{
}

void EntityManager::AddEntity(const std::shared_ptr<Entity> & ent)
{
    entitiesToAdd.push_back(ent);
}

void EntityManager::UpdateAllEntities(double dt)
{
    if (entitiesToAdd.size() > 0)
    {
        for (auto it = entitiesToAdd.begin(); it != entitiesToAdd.end(); ++it)
            entities.push_back(*it);
        entitiesToAdd.clear();
    }

    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        if (!(*it)->IsDead())
            (*it)->Update(dt);

        if ((*it)->IsDead() && (*it).use_count() == 1)
        {
            if (it + 1 == entities.end())
                return RemoveEntity(*it);

            RemoveEntity(*it);
            if (it != entities.begin())
                --it;
        }
    }
}

void EntityManager::RemoveEntity(std::shared_ptr<Entity> & ent)
{
    std::swap(ent, entities.back());
    entities.pop_back();
}

std::shared_ptr<Player> EntityManager::CreatePlayer(MathLib::vec2f position,
                                                    float health)
{
    auto physicsEntity = physicsMgr->CreateEntity(position, 0.2f, {});
    auto entity = factory.CreateEntity<Player>(position, health, physicsEntity);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<Enemy> EntityManager::CreateEnemy(MathLib::vec2f position,
                                                  float health,
                                                  int expCount)
{
    auto physicsEntity = physicsMgr->CreateEntity(position, 0.2f, {});
    auto entity = factory.CreateEntity<Enemy>(position, health, expCount, physicsEntity);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<Projectile> EntityManager::CreateProjectile(MathLib::vec2f position,
                                                            MathLib::vec2f speed,
                                                            float damage,
                                                            double lifetime,
                                                            const LiveEntity* producer)
{
    auto physicsEntity = physicsMgr->CreateEntity(position, 0.2f, speed);
    auto entity = factory.CreateEntity<Projectile>(position, damage, lifetime, producer, physicsEntity);

    AddEntity(entity);
    return entity;
}

std::shared_ptr<ExperiencePoint> EntityManager::CreateExperiencePoint(MathLib::vec2f position,
                                                                      int expCount)
{
    auto physicsEntity = physicsMgr->CreateEntity(position, 0.2f, {});
    auto entity = factory.CreateEntity<ExperiencePoint>(position, expCount, physicsEntity);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<MineDetectorEntity> EntityManager::CreateDetectorMine(const LiveEntity * owner,
                                                                      MathLib::vec2f position,
                                                                      float explosionDamage,
                                                                      float explosionRadius,
                                                                      float triggerDistance)
{
    auto physicsEntity = physicsMgr->CreateEntity(position, 0.2f, {});
    auto entity = factory.CreateEntity<MineDetectorEntity>(owner, position, explosionDamage, explosionRadius, triggerDistance, physicsEntity);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<MineTimedEntity> EntityManager::CreateTimedMine(const LiveEntity * owner,
                                                                MathLib::vec2f position,
                                                                float explosionDamage,
                                                                float explosionRadius,
                                                                float triggerDistance,
                                                                double timeToExplode)
{
    auto physicsEntity = physicsMgr->CreateEntity(position, 0.2f, {});
    auto entity = factory.CreateEntity<MineTimedEntity>(owner, position, explosionDamage, explosionRadius, triggerDistance, timeToExplode, physicsEntity);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<BackgroundBlinker> EntityManager::CreateBackgroundBlinker(ShiftEngine::SpriteSceneNode * sprite)
{
    auto entity = factory.CreateEntity<BackgroundBlinker>(sprite);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<BackgroundWanderer> EntityManager::CreateBackgroundWanderer(ShiftEngine::SpriteSceneNode * sprite)
{
    auto entity = factory.CreateEntity<BackgroundWanderer>(sprite);
    AddEntity(entity);
    return entity;
}

std::shared_ptr<VisualStickerEntity> EntityManager::CreateVisualStickerEntity(const LiveEntity * owner,
                                                                              MathLib::vec2f position,
                                                                              ShiftEngine::SpriteSceneNode * sprite)
{
    auto entity = factory.CreateEntity<VisualStickerEntity>(owner, position, sprite);
    AddEntity(entity);
    return entity;
}
