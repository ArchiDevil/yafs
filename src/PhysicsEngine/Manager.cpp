#include "Manager.h"

#include "Solver.h"

namespace Physics
{

void PhysicsManager::CreateEntity(IPhysicsEntityHolder & holder, const MathLib::vec2f & position, float size, const MathLib::vec2f & speed)
{
    std::shared_ptr<Entity> entity = std::make_shared<Entity>(position, speed, size);
    entities.push_back(entity);
    entity->SetParent(&holder);
    holder.SetEntity(entity);
}

void PhysicsManager::Update(double dt)
{
    Solver::UpdateEntities(dt, entities);
}

}
