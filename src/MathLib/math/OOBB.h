#pragma once

#include "AABB.h"
#include "matrix.h"

namespace MathLib
{

class OOBB
{
public:
    OOBB(vec3f _position = vec3f(), vec3f _size = vec3f(1.0f, 1.0f, 1.0f), vec3f _angles = vec3f());
    void GetVertices(vec3f vertices[8]) const;
    AABB GetAABB() const;

    bool IntersectsAABB(const AABB & bbox);
    bool IntersectsOOBB(const OOBB & bbox);
    bool IntersectsSphere(const vec3f & center, float raduis);
    bool IntersectsLineSegment(const vec3f &mid, const vec3f &dir, const int hl);
    bool IntersectsTriangle(const vec3f &pnt1, const vec3f &pnt2, const vec3f &pnt3);

    vec3f position;
    vec3f size;
    matrix<float, 3> rotation;
};

}
