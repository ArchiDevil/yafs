#include "OOBBCollider.h"

#include <exception>

using namespace Physics;

bool OOBBCollider::CollideWith(ICollider& collider)
{
    return collider.IntersectsWithOOBB(*this);
}

bool OOBBCollider::IntersectsWithSphere(SphereCollider& collider)
{
    throw std::exception("The method or operation is not implemented.");
}

bool OOBBCollider::IntersectsWithOOBB(OOBBCollider& collider)
{
    throw std::exception("The method or operation is not implemented.");
}

bool OOBBCollider::IntersectsWithAABB(AABBCollider& collider)
{
    throw std::exception("The method or operation is not implemented.");
}
