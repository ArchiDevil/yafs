#pragma once

#include <MathLib/math.h>

class Entity;

namespace Physics
{

class Entity
{
public:
    Entity(MathLib::vec2f position, MathLib::vec2f initialSpeed, float size);

    void            Update(double dt, MathLib::vec2f force);
    void            OnCollision(Entity * otherEntity);

    MathLib::vec2f  GetPosition() const;
    MathLib::vec2f  GetSpeed() const;
    float           GetSize() const;
    float           GetElasticity() const;

private:
    MathLib::vec2f position;
    MathLib::vec2f speed;
    float mass = 1.0f;
    float size = 1.0f;
    float elasticity = 0.5f;

};

}
