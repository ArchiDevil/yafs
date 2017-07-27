#pragma once

#include "aabb3.h"
#include "matrix.h"

namespace MathLib
{

template<typename T>
class oobb3
{
public:
    oobb3(vec3<T> position = {},
          vec3<T> size = { 1.0f, 1.0f, 1.0f },
          vec3<T> angles = {})
        : position(position)
        , size(size)
    {
        //	rotation = MatrixPitchYawRoll();
    }

    void GetVertices(vec3<T> vertices[8]) const
    {
        vertices[0] = { position.x - size.x / 2, position.y - size.x / 2, position.z - size.x / 2 };
        vertices[1] = { position.x + size.x / 2, position.y - size.x / 2, position.z - size.x / 2 };
        vertices[2] = { position.x - size.x / 2, position.y + size.x / 2, position.z - size.x / 2 };
        vertices[3] = { position.x - size.x / 2, position.y - size.x / 2, position.z + size.x / 2 };

        vertices[4] = { position.x + size.x / 2, position.y + size.x / 2, position.z - size.x / 2 };
        vertices[5] = { position.x + size.x / 2, position.y - size.x / 2, position.z + size.x / 2 };
        vertices[6] = { position.x - size.x / 2, position.y + size.x / 2, position.z + size.x / 2 };
        vertices[7] = { position.x + size.x / 2, position.y + size.x / 2, position.z + size.x / 2 };

        for (int i = 0; i < 8; i++)
            vertices[i] = rotation * vertices[i];
    }

    aabb3<T> GetAABB() const
    {
        vec3<T> min, max;
        vec3<T> points[8];

        GetVertices(points);

        min.x = points[0].x; min.y = points[0].y; min.z = points[0].z;
        max.x = points[0].x; max.y = points[0].y; max.z = points[0].z;

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (points[i].el[j] < min.el[j])
                    min.el[j] = points[i].el[j];
            }

            for (int j = 0; j < 3; j++)
            {
                if (points[i].el[j] < max.el[j])
                    max.el[j] = points[i].el[j];
            }
        }

        return { min, max };
    }

    vec3<T> position;
    vec3<T> size;
    matrix<T, 3> rotation;
};

using oobb3f = oobb3<float>;
using oobb3d = oobb3<double>;

}
