#pragma once

#include "vector3.h"

namespace MathLib
{

template<typename T>
struct ray3
{
    constexpr ray3() = default;
    constexpr ray3(const ray3&) = default;

    constexpr ray3(const vec3<T> & origin,
                   const vec3<T> & direction)
        : origin(origin)
        , direction(direction)
    {
    }

    vec3<T> origin;
    vec3<T> direction;
};

using ray3f = ray3<float>;
using ray3d = ray3<double>;

}
