#include "Entity.h"

#include "Types.h"

#include <cassert>

Physics::Entity::Entity(MathLib::vec2f position, MathLib::vec2f initialSpeed, float size)
    : position(position)
    , velocity(initialSpeed)
    , size(size)
{
}

MathLib::vec2f Physics::Entity::GetPosition() const
{
    return position;
}

MathLib::vec2f Physics::Entity::GetVelocity() const
{
    return velocity;
}

float Physics::Entity::GetSize() const
{
    return size;
}

float Physics::Entity::GetElasticity() const
{
    return elasticity;
}

Physics::IPhysicsEntityHolder * Physics::Entity::GetParent() const
{
    return parent;
}

void Physics::Entity::SetParent(Physics::IPhysicsEntityHolder * holder)
{
    parent = holder;
}

void Physics::Entity::SetPosition(MathLib::vec2f pos)
{
    position = pos;
}

void Physics::Entity::SetVelocity(MathLib::vec2f v)
{
    velocity = v;
}

void Physics::Entity::ApplyForce(MathLib::vec2f f)
{
    instantForce = f;
}

void Physics::Entity::Update(double dt, MathLib::vec2f force)
{
    // mass here is just for the future
    // for now, it is permanently 1.0
    MathLib::vec2d resultForce = force + instantForce;
    MathLib::vec2d acceleration = resultForce * mass;
    velocity += acceleration * (float)dt;
    position += velocity * (float)dt;

    instantForce = {};
}

void Physics::Entity::OnCollision(Entity * otherEntity)
{
    assert(otherEntity);
    assert(otherEntity->parent);
    assert(parent);

    parent->OnCollision(otherEntity->GetParent());
}
