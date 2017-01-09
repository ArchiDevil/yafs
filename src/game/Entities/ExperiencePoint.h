#pragma once

#include "Entity.h"

#include <PhysicsEngine/PhysicsEngine.h>

class ExperiencePoint final
    : public Entity
    , public Physics::IPhysicsEntityHolder
{

public:
    ExperiencePoint(MathLib::vec2f position,
                    int expCount,
                    const std::shared_ptr<Physics::Entity>& physicsEntity,
                    float size);

    void Update(double dt) override;
    int GetExperienceCount() const;

    // Physics::IPhysicsEntityHolder
    void OnCollision(IPhysicsEntityHolder* holder) override;

private:
    int expCount;
    double remainingTime = 10.0;
};
