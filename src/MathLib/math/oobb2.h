#pragma once

#include "vector2.h"
#include "line2.h"
#include "matrixFuncs.h"

namespace MathLib
{

template<typename T>
class oobb2
{
    oobb2(vec2<T> center, vec2<T> sizes, T angle)
        : center(center)
        , sizes(sizes)
        , angle(angle)
    {
    }

    vec2<T> center;
    vec2<T> sizes;
    T angle;
};

}
