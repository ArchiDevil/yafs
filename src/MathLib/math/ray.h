#pragma once

#include "vector3.h"

namespace MathLib
{

struct Ray
{
    constexpr Ray(const vec3f & origin = {}, const vec3f & direction = {})
        : Origin(origin)
        , Direction(direction)
    {
    }

    vec3f Origin;
    vec3f Direction;
};

}
