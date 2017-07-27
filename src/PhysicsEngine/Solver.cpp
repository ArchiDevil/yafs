#include "Solver.h"

#include <MathLib/math.h>

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

        UpdateEntity(dt, index, *entities[index].lock(), entities);
    }
}

void Physics::Solver::UpdateEntity(double dt, size_t currentIndex, Entity& entity, std::vector<std::weak_ptr<Entity>>& entities)
{
    MathLib::vec2f resultingForce;

    for (size_t index = 0; index < entities.size(); ++index)
    {
        if(index == currentIndex)
            continue;

        std::shared_ptr<Entity> entityToCheckWith = entities[index].lock();
        if (!entityToCheckWith)  // we don't modify entities list here, every modification will be completed on upper level
            continue;            // this is not fast method to complete updating, due to huge time lock() requires, but for now
                                 // I think it works fine

        float resultingSize = entity.GetSize() + entityToCheckWith->GetSize();
        float distance = MathLib::distance(entity.GetPosition(), entityToCheckWith->GetPosition());

        if (distance < resultingSize) // collision!
        {
            float distanceToSecondBorder = distance - entityToCheckWith->GetSize();
            assert(distanceToSecondBorder < entity.GetSize());

            float penetration = entity.GetSize() - distanceToSecondBorder;

            // force is calculated with Hooke's law
            float forcePower = penetration * entity.GetElasticity();
            MathLib::vec2f forceDirection = entity.GetPosition() - entityToCheckWith->GetPosition();

            resultingForce += forceDirection * forcePower;
            entity.OnCollision(entityToCheckWith.get());
        }
    }

    const float frictionCoef = 0.1f;
    resultingForce += -entity.GetVelocity() * frictionCoef;

    entity.Update(dt, resultingForce);
}
