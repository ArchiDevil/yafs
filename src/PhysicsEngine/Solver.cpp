#include "Solver.h"

#include <cassert>

void Physics::Solver::UpdateEntities(double dt, std::vector<std::weak_ptr<Entity>>& entities)
{
    for (size_t index = 0; index < entities.size(); ++index)
    {
        if (entities[index].expired())
        {
            std::swap(entities[index], entities.back());
            entities.pop_back();
            index--;
            continue;
        }

        UpdateEntity(dt, *entities[index].lock(), entities);
    }
}

void Physics::Solver::UpdateEntity(double dt, Entity& entity, std::vector<std::weak_ptr<Entity>>& entities)
{
    MathLib::vec2f resultingForce;

    for (size_t index = 0; index < entities.size(); ++index)
    {
        std::shared_ptr<Entity> entityToCheckWith = entities[index].lock();
        if (!entityToCheckWith)  // we don't modify entities list here, every modification will be completed on upper level
            continue;            // this is not fast method to complete updating, due to huge time lock() requires, but for now
                                 // I think it works fine

        float resultingSize = entity.GetSize() + entityToCheckWith->GetSize();

        if (MathLib::distance(entity.GetPosition(), entityToCheckWith->GetPosition()) < resultingSize) // collision!
        {
            float distanceToSecondBorder = resultingSize - entityToCheckWith->GetSize();
            assert(distanceToSecondBorder < entity.GetSize());

            float penetrationDistance = entity.GetSize() - distanceToSecondBorder;

            // force is calculated with Hooke's law
            float forcePower = penetrationDistance * entity.GetElasticity();
            MathLib::vec2f forceDirection = entity.GetPosition() - entityToCheckWith->GetPosition();

            resultingForce += forceDirection * forcePower;
            entity.OnCollision(entityToCheckWith.get());
        }
    }

    entity.Update(dt, resultingForce);
}
