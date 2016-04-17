#pragma once

#include "vector3.h"

namespace MathLib
{

struct Ray
{
    Ray(const vec3f & origin = vec3f(0.0f, 0.0f, 0.0f),
        const vec3f & direction = vec3f(0.0f, 0.0f, 0.0f))
        : Origin(origin)
        , Direction(direction)
    {}

    vec3f Origin;
    vec3f Direction;
};

}
