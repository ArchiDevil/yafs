#include "AABBCollider.h"

#include <exception>

using namespace Physics;

bool AABBCollider::CollideWith(ICollider& collider)
{
    return collider.IntersectsWithAABB(*this);
}

bool AABBCollider::IntersectsWithSphere(SphereCollider& collider)
{
    throw std::exception("The method or operation is not implemented.");
}

bool AABBCollider::IntersectsWithOOBB(OOBBCollider& collider)
{
    throw std::exception("The method or operation is not implemented.");
}

bool AABBCollider::IntersectsWithAABB(AABBCollider& collider)
{
    throw std::exception("The method or operation is not implemented.");
}
