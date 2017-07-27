#pragma once

#include "ICollider.h"

namespace Physics
{

class AABBCollider final : public ICollider
{
public:
    bool CollideWith(ICollider& collider) override;

    bool IntersectsWithSphere(SphereCollider& collider) override;
    bool IntersectsWithOOBB(OOBBCollider& collider) override;
    bool IntersectsWithAABB(AABBCollider& collider) override;

};

}
