#include "Entity.h"

Physics::Entity::Entity(MathLib::vec2f position, MathLib::vec2f initialSpeed)
    : position(position)
    , speed(initialSpeed)
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

void Physics::Entity::Update(double dt, MathLib::vec2f force)
{
    // mass here is just for the future
    // for now, it is permanently 1.0
    MathLib::vec2d acceleration = force * mass;
    speed += acceleration * (float)dt;
    position += speed * (float)dt;
}
