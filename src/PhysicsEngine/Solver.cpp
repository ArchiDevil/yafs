#include "Solver.h"

void Physics::Solver::UpdateEntities(double dt, std::vector<std::weak_ptr<Entity>>& entities)
{
    for (size_t index = 0; index < entities.size(); ++index)
    {
        if (entities[index].expired())
        {
            index--;
            std::swap(entities.begin(), entities.end() - 1);
            entities.pop_back();
            continue;
        }

        entities[index].lock()->Update(dt, {});
    }
}

