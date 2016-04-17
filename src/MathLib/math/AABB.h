#pragma once

#include <cmath>
#include <algorithm>

#include "vector3.h"

namespace MathLib
{

class AABB
{
public:
    AABB(const vec3f & _bmin = vec3f(0.0f, 0.0f, 0.0f),
         const vec3f & _bmax = vec3f(0.0f, 0.0f, 0.0f));

    void GetVertices(vec3f vertices[8]) const;

    bool IntersectsAABB(const AABB & otherAABBox) const;
    bool IntersectsLineSegment(const vec3f &mid, const vec3f &dir, const int hl) const;

    void Include(const AABB & otherAABBox);
    float GetSurfaceArea() const;
    float GetVolume() const;

    vec3f GetCentralPoint() const;

    vec3f bMin;
    vec3f bMax;
};

}
