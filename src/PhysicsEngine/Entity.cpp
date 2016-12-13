#include "Entity.h"

#include <cassert>

Physics::Entity::Entity(MathLib::vec2f position, MathLib::vec2f initialSpeed, float size)
    : position(position)
    , speed(initialSpeed)
    , size(size)
{
}

MathLib::vec2f Physics::Entity::GetPosition() const
{
    return position;
}

MathLib::vec2f Physics::Entity::GetSpeed() const
{
    return speed;
}

float Physics::Entity::GetSize() const
{
    return size;
}

float Physics::Entity::GetElasticity() const
{
    return elasticity;
}

void Physics::Entity::Update(double dt, MathLib::vec2f force)
{
    // mass here is just for the future
    // for now, it is permanently 1.0
    MathLib::vec2d acceleration = force * mass;
    speed += acceleration * (float)dt;
    position += speed * (float)dt;
}

void Physics::Entity::OnCollision(Entity * otherEntity)
{
    assert(otherEntity);
    notifier<CollisionEvent>::notifyAll(CollisionEvent{ this, otherEntity });
}
