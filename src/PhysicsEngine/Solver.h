#pragma once

#include <Utilities/IManager.h>

#include <memory>
#include <vector>

#include "Entity.h"

namespace Physics
{

class Solver : public IManager
{
public:
    void UpdateEntities(double dt, std::vector<std::weak_ptr<Entity>>& entities);

private:
    void UpdateEntity(double dt, Entity& entity, std::vector<std::weak_ptr<Entity>>& entities);
};

}
