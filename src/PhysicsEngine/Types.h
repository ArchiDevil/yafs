#pragma once

#include "Entity.h"

#include <memory>

namespace Physics
{

class Entity;

class IPhysicsEntityHolder
{
public:
    virtual void OnCollision(IPhysicsEntityHolder* /*other*/) {}
    void SetEntity(std::shared_ptr<Entity> entity) { physicsEntity = entity; }

protected:
    std::shared_ptr<Entity> physicsEntity;
};

}
