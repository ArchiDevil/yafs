#pragma once

#include "vector2.h"

namespace MathLib
{

template<typename T>
class oobb2d
{
    oobb2d(vec2<T> center, vec2<T> sizes, T angle)
        : center(center)
        , sizes(sizes)
        , angle(angle)
    {
    }

    bool intersectsWithPoint(vec2f<T> point)
    {
        return false;
    }

    bool intersectsWithSphere(vec2f<T> center, T radius)
    {
        return false;
    }

    vec2<T> center;
    vec2<T> sizes;
    T angle;
};

}
