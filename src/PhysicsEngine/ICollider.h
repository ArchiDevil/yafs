#pragma once

namespace Physics
{

class SphereCollider;
class OOBBCollider;
class AABBCollider;

class ICollider
{
public:
    virtual bool CollideWith(ICollider& collider) = 0;

    virtual bool IntersectsWithSphere(SphereCollider& collider) = 0;
    virtual bool IntersectsWithOOBB(OOBBCollider& collider) = 0;
    virtual bool IntersectsWithAABB(AABBCollider& collider) = 0;
};

}
