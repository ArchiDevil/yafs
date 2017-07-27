#pragma once

#include "ICollider.h"

#include <MathLib/math.h>

namespace Physics
{

class SphereCollider final : public ICollider
{
public:
    SphereCollider(MathLib::vec2f center, float radius);

    bool CollideWith(ICollider& collider) override;

    bool IntersectsWithSphere(SphereCollider& collider) override;
    bool IntersectsWithOOBB(OOBBCollider& collider) override;
    bool IntersectsWithAABB(AABBCollider& collider) override;

private:
    float radius;
    MathLib::vec2f center;

};

}
