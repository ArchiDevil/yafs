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
    void UpdateAllEntities(double dt);

    std::shared_ptr<Player>             CreatePlayer(const MathLib::vec2f & position,
                                                     float health);

    std::shared_ptr<Enemy>              CreateEnemy(const MathLib::vec2f & position,
                                                    float health,
                                                    int expCount,
                                                    Enemy::EnemyType type);

    std::shared_ptr<Projectile>         CreateProjectile(const MathLib::vec2f & position,
                                                         const MathLib::vec2f & speed,
                                                         float damage,
                                                         double lifetime,
                                                         const LiveEntity* producer);

    std::shared_ptr<MineDetectorEntity> CreateDetectorMine(const LiveEntity * owner,
                                                           const MathLib::vec2f& position,
                                                           float explosionDamage,
                                                           float explosionRadius,
                                                           float triggerDistance);

    std::shared_ptr<MineTimedEntity>    CreateTimedMine(const LiveEntity * owner,
                                                        const MathLib::vec2f& position,
                                                        float explosionDamage,
                                                        float explosionRadius,
                                                        float triggerDistance,
                                                        double timeToExplode);

    std::shared_ptr<ExperiencePoint>    CreateExperiencePoint(const MathLib::vec2f & position,
                                                              int expCount);

    std::shared_ptr<BackgroundBlinker>  CreateBackgroundBlinker(ShiftEngine::SpriteSceneNode * sprite);

    std::shared_ptr<BackgroundWanderer> CreateBackgroundWanderer(ShiftEngine::SpriteSceneNode * sprite);

    std::shared_ptr<VisualStickerEntity>CreateVisualStickerEntity(const LiveEntity * owner,
                                                                  const MathLib::vec2f& position,
                                                                  ShiftEngine::SpriteSceneNode * sprite);

    std::vector<LiveEntity*> GetHostileLiveEntities(LiveEntity::Faction fraction) const;
    std::vector<Projectile*> GetProjectiles() const;

private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Entity>> entitiesToAdd;

    // this is here just for fast returning result in GetHostileLiveEntities
    std::vector<LiveEntity*> liveEntities;
    std::vector<Projectile*> projectileEntities;

    EntityFactory factory;

};
