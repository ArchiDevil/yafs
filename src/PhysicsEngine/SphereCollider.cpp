#include "SphereCollider.h"

#include <exception>

using namespace Physics;

Physics::SphereCollider::SphereCollider(MathLib::vec2f center, float radius)
    : center(center)
    , radius(radius)
{
}

bool SphereCollider::CollideWith(ICollider& collider)
{
    return collider.IntersectsWithSphere(*this);
}

bool SphereCollider::IntersectsWithSphere(SphereCollider& collider)
{
    return MathLib::distance(this->center, collider.center) < (this->radius + collider.radius);
}

bool SphereCollider::IntersectsWithOOBB(OOBBCollider& collider)
{
    throw std::exception("The method or operation is not implemented.");
}

bool SphereCollider::IntersectsWithAABB(AABBCollider& collider)
{
    throw std::exception("The method or operation is not implemented.");
}
