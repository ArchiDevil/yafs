#pragma once

#include <MathLib/math.h>

namespace Physics
{

class Entity
{
public:
    Entity(MathLib::vec2f position, MathLib::vec2f initialSpeed);

    void Update(double dt, MathLib::vec2f force);

    MathLib::vec2f GetPosition() const;
    MathLib::vec2f GetSpeed() const;

private:
    MathLib::vec2f position;
    MathLib::vec2f speed;
    float mass = 1.0f;

};

}
