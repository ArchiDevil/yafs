#pragma once

#include <MathLib/math.h>

namespace Physics
{

class Entity;
class IPhysicsEntityHolder;

class Entity
{
public:
    Entity(MathLib::vec2f position, MathLib::vec2f initialSpeed, float size);

    void                    Update(double dt, MathLib::vec2f force);
    void                    OnCollision(Entity * otherEntity);

    MathLib::vec2f          GetPosition() const;
    MathLib::vec2f          GetVelocity() const;
    float                   GetSize() const;
    float                   GetElasticity() const;
    IPhysicsEntityHolder *  GetParent() const;

    void                    SetParent(IPhysicsEntityHolder * holder);
    void                    SetPosition(MathLib::vec2f pos);
    void                    SetVelocity(MathLib::vec2f v);
    void                    ApplyForce(MathLib::vec2f f);

private:
    MathLib::vec2f          position;
    MathLib::vec2f          velocity;
    float                   mass = 1.0f;
    float                   size = 1.0f;
    float                   elasticity = 0.5f;
    IPhysicsEntityHolder *  parent = nullptr;

    MathLib::vec2f          instantForce;

};

}
