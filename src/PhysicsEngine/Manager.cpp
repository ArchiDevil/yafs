#include "Manager.h"

namespace Physics
{

std::shared_ptr<Entity> PhysicsManager::CreateEntity(const MathLib::vec2f & position, float size, const MathLib::vec2f & speed)
{
    std::shared_ptr<Entity> entity = std::make_shared<Entity>(position, speed, size);
    entities.push_back(entity);
    return entity;
}

void PhysicsManager::Update(double dt, Solver * solver)
{
    if (!solver)
        return;

    solver->UpdateEntities(dt, entities);
}

}
