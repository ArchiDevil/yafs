#pragma once

#include <cmath>
#include <algorithm>

#include "vector3.h"
#include "oobb2.h"

namespace MathLib
{

template<typename T>
class aabb2
{
public:
    constexpr aabb2() = default;
    constexpr aabb2(const aabb2&) = default;

    constexpr aabb2(vec2<T> position, vec2<T> sizes)
        : position(position)
        , sizes(sizes)
    {
    }

    vec2<T> position;
    vec2<T> sizes;
};

}
