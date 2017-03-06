#pragma once

#include "Entity.h"

#include <PhysicsEngine/Types.h>

class PhysicsEntity
    : public Entity
    , public Physics::IPhysicsEntityHolder
{
public:
    PhysicsEntity(const MathLib::vec2f & position,
                  ShiftEngine::SpriteSceneNode * sprite,
                  const std::shared_ptr<Physics::Entity>& physicsEntity);
    virtual ~PhysicsEntity() = default;

    virtual void            Update(double dt) override;
    virtual MathLib::vec2f  GetPosition() const override;
    virtual void            SetPosition(MathLib::vec2f pos) override;

};
