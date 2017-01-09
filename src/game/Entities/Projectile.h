#pragma once

#include "LiveEntity.h"

#include <MathLib/math.h>

#include <PhysicsEngine/PhysicsEngine.h>

class Projectile final
    : public Entity
    , public Physics::IPhysicsEntityHolder
{
public:
    Projectile(const MathLib::vec2f position,
               float damage,
               double lifetime,
               const LiveEntity * producer,
               const std::shared_ptr<Physics::Entity>& physicsEntity,
               float size);

    virtual ~Projectile() = default;

    const LiveEntity *  GetProducer() const;

    // Entity
    void                Update(double dt) override;

    // Physics::IPhysicsEntityHolder
    void                OnCollision(Physics::IPhysicsEntityHolder* other) override;

private:
    double              remainingTime = 0.0;
    float               damage = 1.0f;
    const LiveEntity*   producer = nullptr;

};
