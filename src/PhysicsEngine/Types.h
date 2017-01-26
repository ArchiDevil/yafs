#pragma once

#include "Entity.h"

#include <memory>

namespace Physics
{

class Entity;

class IPhysicsEntityHolder
{
public:
    IPhysicsEntityHolder(const std::shared_ptr<Entity>& entity)
        : physicsEntity(entity)
    {
        if (entity) entity->SetParent(this);
    }

    virtual void OnCollision(IPhysicsEntityHolder* /*other*/) {}

protected:
    // TODO: consider removing shared ptr and changing it to something more lightweight
    std::shared_ptr<Entity> physicsEntity;
};

}
