#pragma once

#include "vector2.h"
#include "line2d.h"
#include "matrixFuncs.h"

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

    bool intersectsWithPoint(vec2<T> point)
    {
        vec2<T> xDir{ 1.0, 0.0 };
        vec2<T> yDir{ 0.0, 1.0 };

        matrix<T, 4> rotator = matrixRotationZ(angle);
        xDir = normalize(vec2Transform(xDir, rotator));
        yDir = normalize(vec2Transform(yDir, rotator));

        line2d<T> lines[] = {
            lineFromPointAndNormalVector(xDir + sizes.x,  xDir),
            lineFromPointAndNormalVector(xDir - sizes.x, -xDir),
            lineFromPointAndNormalVector(yDir + sizes.y,  yDir),
            lineFromPointAndNormalVector(yDir - sizes.y, -xDir)
        };

        return false;
    }

    bool intersectsWithSphere(vec2<T> center, T radius)
    {
        return false;
    }

    vec2<T> center;
    vec2<T> sizes;
    T angle;
};

}
