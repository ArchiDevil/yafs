#pragma once

#include "Entity.h"
#include "EntityFactory.h"

#include <memory>
#include <Utilities/IManager.h>
#include <Utilities/singleton.h>
#include <vector>

class EntityManager final : public IManager
{
public:
    EntityManager() = default;
    ~EntityManager() = default;

    void UpdateAllEntities(double dt);

    std::shared_ptr<Player> CreatePlayer(const MathLib::vec2f & position, float health);
    std::shared_ptr<Enemy> CreateEnemy(const MathLib::vec2f & position, float health, int expCount, const std::shared_ptr<AIBase> & ai);
    std::shared_ptr<Projectile> CreateProjectile(const MathLib::vec2f & position, const MathLib::vec2f & speed, float damage, double lifetime, Entity* producer);
    std::shared_ptr<ExperiencePoint> CreateExperiencePoint(const MathLib::vec2f & position, int expCount);
    std::shared_ptr<BackgroundBlinker> CreateBackgroundBlinker(ShiftEngine::SpriteSceneNode *sprite);
    std::shared_ptr<BackgroundWanderer> CreateBackgroundWanderer(ShiftEngine::SpriteSceneNode *sprite);

    const std::vector<std::shared_ptr<LiveEntity>> * GetLiveEntities();
    const std::vector<std::shared_ptr<LiveEntity>> * GetHostileLiveEntities(LiveEntity::Fraction fraction);
private:
    template<typename T>
    void AddEntity(const std::shared_ptr<T> & ent, std::vector<std::shared_ptr<T>> & list);
    template<typename T>
    void RemoveEntity(std::shared_ptr<T> & ent, std::vector<std::shared_ptr<T>> & list);

    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Entity>> entitiesToAdd;
    std::vector<std::shared_ptr<LiveEntity>> liveEntities;
    std::vector<std::shared_ptr<LiveEntity>> liveEntitiesToAdd;
    std::shared_ptr<EntityFactory> factory = std::make_shared<EntityFactory>();

};
