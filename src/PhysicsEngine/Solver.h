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
    static void UpdateEntities(double dt, std::vector<std::weak_ptr<Entity>>& entities);

private:
    static void UpdateEntity(double dt, size_t currentIndex, Entity& entity, std::vector<std::weak_ptr<Entity>>& entities);
};

}
