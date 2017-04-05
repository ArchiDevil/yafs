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
        // we need 3x3 matrix here, not 4x4
        matrix<T, 3> translationMatrix = matrixTranslation(center.x, center.y);
        matrix<T, 3> scalingMatrix = matrixScaling(sizes.x, sizes.y);
        matrix<T, 3> rotationMatrix = matrixRotationZ(angle);
        matrix<T, 3> total = scalingMatrix * rotationMatrix * translationMatrix;
        point = normalize(vec2Transform(point, total));

        return (point.x >= -0.5f || point.x <= 0.5f)
            && (point.y >= -0.5f || point.y <= 0.5f);
    }

    bool intersectsWithSphere(vec2<T> sphereCenter, T sphereRadius)
    {
        matrix<T, 3> translationMatrix = matrixTranslation(center.x, center.y);
        matrix<T, 3> scalingMatrix = matrixScaling(sizes.x, sizes.y);
        matrix<T, 3> rotationMatrix = matrixRotationZ(angle);
        matrix<T, 3> total = scalingMatrix * rotationMatrix * translationMatrix;
        point = normalize(vec2Transform(sphereCenter, total));

        return false;
    }

    vec2<T> center;
    vec2<T> sizes;
    T angle;
};

}
