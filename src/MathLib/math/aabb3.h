#pragma once

#include <cmath>
#include <algorithm>

#include "vector3.h"

namespace MathLib
{

template<typename T>
class aabb3
{
public:
    aabb3(const vec3f & bmin = {},
          const vec3f & bmax = {})
        : bMin(bmin)
        , bMax(bmax)
    {
    }

    void getVertices(vec3f vertices[8]) const
    {
        vertices[0] = { bMin };
        vertices[1] = { bMax.x, bMin.y, bMin.z };
        vertices[2] = { bMin.x, bMax.y, bMin.z };
        vertices[3] = { bMax.x, bMax.y, bMin.z };

        vertices[4] = { bMin.x, bMin.y, bMax.z };
        vertices[5] = { bMin.x, bMax.y, bMax.z };
        vertices[6] = { bMax.x, bMin.y, bMax.z };
        vertices[7] = { bMax };
    }

    void include(const aabb3 & other)
    {
        bMin.x = std::min(bMin.x, other.bMin.x);
        bMin.y = std::min(bMin.y, other.bMin.y);
        bMin.z = std::min(bMin.z, other.bMin.z);

        bMax.x = std::max(bMax.x, other.bMax.x);
        bMax.y = std::max(bMax.y, other.bMax.y);
        bMax.z = std::max(bMax.z, other.bMax.z);
    }

    float getSurfaceArea() const
    {
        float l = std::abs(bMax.x - bMin.x);
        float h = std::abs(bMax.y - bMin.y);
        float w = std::abs(bMax.z - bMin.z);

        return 2.0f * (l * w + l * h + w * h);
    }

    float getVolume() const
    {
        float l = bMax.x - bMin.x;
        float h = bMax.y - bMin.y;
        float w = bMax.z - bMin.z;

        return std::abs(l * h * w);
    }

    vec3f getCentralPoint() const
    {
        return vec3f((bMax.x - bMin.x) / 2.0f,
            (bMax.y - bMin.y) / 2.0f,
                     (bMax.z - bMin.z) / 2.0f) + bMin;
    }

    vec3f bMin;
    vec3f bMax;
};

using aabb3f = aabb3<float>;
using aabb3d = aabb3<double>;

}
