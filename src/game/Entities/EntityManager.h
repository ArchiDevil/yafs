#pragma once

#include "Entity.h"
#include "EntityFactory.h"

#include <Utilities/IManager.h>
#include <Utilities/singleton.h>

#include <vector>
#include <memory>

class EntityManager final
    : public IManager
{
public:
    EntityManager(Physics::PhysicsManager* physicsMgr);

    void UpdateAllEntities(double dt);

    std::shared_ptr<Player>             CreatePlayer(MathLib::vec2f position,
                                                     float health,
                                                     float size);

    std::shared_ptr<Enemy>              CreateEnemy(MathLib::vec2f position,
                                                    float health,
                                                    int expCount,
                                                    float size);

    std::shared_ptr<Projectile>         CreateProjectile(MathLib::vec2f position,
                                                         MathLib::vec2f speed,
                                                         float damage,
                                                         double lifetime,
                                                         const LiveEntity* producer,
                                                         float size);

    std::shared_ptr<ExperiencePoint>    CreateExperiencePoint(MathLib::vec2f position,
                                                              int expCount,
                                                              float size);

    std::shared_ptr<MineDetectorEntity> CreateDetectorMine(const LiveEntity * owner,
                                                           MathLib::vec2f position,
                                                           float explosionDamage,
                                                           float explosionRadius,
                                                           float triggerDistance,
                                                           float size);

    std::shared_ptr<MineTimedEntity>    CreateTimedMine(const LiveEntity * owner,
                                                        MathLib::vec2f position,
                                                        float explosionDamage,
                                                        float explosionRadius,
                                                        float triggerDistance,
                                                        double timeToExplode,
                                                        float size);

    std::shared_ptr<BackgroundBlinker>  CreateBackgroundBlinker(ShiftEngine::SpriteSceneNode * sprite);

    std::shared_ptr<BackgroundWanderer> CreateBackgroundWanderer(ShiftEngine::SpriteSceneNode * sprite);

    std::shared_ptr<VisualStickerEntity>CreateVisualStickerEntity(const LiveEntity * owner,
                                                                  MathLib::vec2f position,
                                                                  ShiftEngine::SpriteSceneNode * sprite);

private:
    void AddEntity(const std::shared_ptr<Entity> & ent);
    void RemoveEntity(std::shared_ptr<Entity> & ent);

    Physics::PhysicsManager * physicsMgr = nullptr;

    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Entity>> entitiesToAdd;
    EntityFactory factory;

};
