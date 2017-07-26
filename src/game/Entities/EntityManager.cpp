#include "EntityManager.h"

#include "../AI/AISmallSpirit.h"
#include "../Game.h"
#include "../Spells/SpellsDatabase.h"

#include <algorithm>

using namespace MathLib;

EntityManager::EntityManager(Physics::PhysicsManager* physicsMgr)
    : physicsMgr(physicsMgr)
{
}

template<typename T>
void RemoveEntity(std::shared_ptr<T> & ent,
                  std::vector<std::shared_ptr<T>> & list,
                  std::vector<LiveEntity*> & liveReferences,
                  std::vector<Projectile*> & projectileReferences)
{
    LiveEntity* live = dynamic_cast<LiveEntity*>(ent.get());
    auto liveIter = std::find(liveReferences.begin(),
                              liveReferences.end(),
                              live);
    if (liveIter != liveReferences.end())
        liveReferences.erase(liveIter);

    Projectile* proj = dynamic_cast<Projectile*>(ent.get());
    auto projIter = std::find(projectileReferences.begin(),
                              projectileReferences.end(),
                              proj);
    if (projIter != projectileReferences.end())
        projectileReferences.erase(projIter);

    std::swap(ent, list.back());
    list.pop_back();
}

void EntityManager::UpdateAllEntities(double dt)
{
    if (entitiesToAdd.size() > 0)
    {
        for (auto& entity : entitiesToAdd)
            entities.push_back(entity);
        entitiesToAdd.clear();
    }

    for (size_t i = 0; i < entities.size(); ++i)
    {
        if (!entities[i]->IsDead())
            entities[i]->Update(dt);

        // why do we need the second condition here?
        // does it matter that anyone has a reference on an entity or not?
        if (entities[i]->IsDead() && entities[i].use_count() == 1)
        {
            RemoveEntity(entities[i], entities, liveEntities, projectileEntities);
            --i;
        }
    }
}

std::vector<LiveEntity*> EntityManager::GetHostileLiveEntities(LiveEntity::Faction faction) const
{
    std::vector<LiveEntity*> hostileEnt;

    for (auto& entity : liveEntities)
        if (entity->GetFaction() != faction)
            hostileEnt.push_back(entity);

    return hostileEnt;
}

std::shared_ptr<Player> EntityManager::CreatePlayer(MathLib::vec2f position,
                                                    float health,
                                                    float size)
{
    auto physicsEntity = physicsMgr->CreateEntity(position, size, {});
    auto entity = factory.CreateEntity<Player>(position, health, physicsEntity);
    entitiesToAdd.push_back(entity);
    liveEntities.push_back(entity.get());
    return entity;
}

std::shared_ptr<Enemy> EntityManager::CreateEnemy(MathLib::vec2f position,
                                                  float health,
                                                  int expCount,
                                                  float size,
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

    auto physicsEntity = physicsMgr->CreateEntity(position, size, {});
    auto entity = factory.CreateEntity<Enemy>(position, health, expCount, physicsEntity, std::move(ai));
    entity->SetSpellController(GoingHome::GetGamePtr()->GetSpellsDatabase()->GetSpellByName("projectile").CreateSpellController(entity.get()), Player::CS_MainSlot);
    entitiesToAdd.push_back(entity);
    liveEntities.push_back(entity.get());
    return entity;
}

std::vector<Projectile*> EntityManager::GetProjectiles() const
{
    return projectileEntities;
}

std::shared_ptr<Projectile> EntityManager::CreateProjectile(MathLib::vec2f position,
                                                            MathLib::vec2f speed,
                                                            float damage,
                                                            double lifetime,
                                                            const LiveEntity* producer,
                                                            float size)
{
    auto physicsEntity = physicsMgr->CreateEntity(position, size, speed);
    auto entity = factory.CreateEntity<Projectile>(position, damage, lifetime, producer, physicsEntity, size);
    entitiesToAdd.push_back(entity);
    projectileEntities.push_back(entity.get());
    return entity;
}

std::shared_ptr<ExperiencePoint> EntityManager::CreateExperiencePoint(MathLib::vec2f position,
                                                                      int expCount,
                                                                      float size)
{
    auto physicsEntity = physicsMgr->CreateEntity(position, size, {});
    auto entity = factory.CreateEntity<ExperiencePoint>(position, expCount, physicsEntity, size);
    entitiesToAdd.push_back(entity);
    return entity;
}

std::shared_ptr<MineDetectorEntity> EntityManager::CreateDetectorMine(const LiveEntity * owner,
                                                                      MathLib::vec2f position,
                                                                      float explosionDamage,
                                                                      float explosionRadius,
                                                                      float triggerDistance,
                                                                      float size)
{
    auto physicsEntity = physicsMgr->CreateEntity(position, size, {});
    auto entity = factory.CreateEntity<MineDetectorEntity>(owner, position, explosionDamage, explosionRadius, triggerDistance, physicsEntity);
    entitiesToAdd.push_back(entity);
    return entity;
}

std::shared_ptr<MineTimedEntity> EntityManager::CreateTimedMine(const LiveEntity * owner,
                                                                MathLib::vec2f position,
                                                                float explosionDamage,
                                                                float explosionRadius,
                                                                float triggerDistance,
                                                                double timeToExplode,
                                                                float size)
{
    auto physicsEntity = physicsMgr->CreateEntity(position, size, {});
    auto entity = factory.CreateEntity<MineTimedEntity>(owner, position, explosionDamage, explosionRadius, triggerDistance, timeToExplode, physicsEntity);
    entitiesToAdd.push_back(entity);
    return entity;
}

std::shared_ptr<VisualStickerEntity> EntityManager::CreateVisualStickerEntity(const LiveEntity * owner,
                                                                              MathLib::vec2f position,
                                                                              ShiftEngine::SpriteSceneNode * sprite)
{
    auto entity = factory.CreateEntity<VisualStickerEntity>(owner, position, sprite);
    entitiesToAdd.push_back(entity);
    return entity;
}
