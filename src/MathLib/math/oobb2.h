#pragma once

#include "vector2.h"
#include "line2.h"
#include "matrixFuncs.h"

namespace MathLib
{

template<typename T>
class oobb2
{
public:
    oobb2(vec2<T> position, vec2<T> sizes, T angle)
        : position(position)
        , sizes(sizes)
        , angle(angle)
    {
    }

    vec2<T> position;
    vec2<T> sizes;
    T angle;
};

using oobb2f = oobb2<float>;
using oobb2d = oobb2<double>;

}
