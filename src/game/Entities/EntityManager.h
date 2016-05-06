#pragma once

#include "Entity.h"

#include <memory>
#include <Utilities/singleton.h>
#include <vector>

class EntityManager
    : public singleton <EntityManager>
{
public:
    EntityManager() {}
    virtual ~EntityManager() {}

    void AddEntity(const std::shared_ptr<Entity> & ent);
    void RemoveEntity(const std::shared_ptr<Entity> & ent);

    void UpdateAllEntities(double dt);

private:
    std::vector<std::shared_ptr<Entity>> entities;
};