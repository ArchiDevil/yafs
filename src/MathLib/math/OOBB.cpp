#include "OOBB.h"

MathLib::OOBB::OOBB(vec3f _position, vec3f _size, vec3f /*_angles*/)
    : position(_position)
    , size(_size)
{
    //	rotation = MatrixPitchYawRoll();
}

void MathLib::OOBB::GetVertices(vec3f vertices[8]) const
{
    vertices[0] = vec3f(position.x - size.x / 2, position.y - size.x / 2, position.z - size.x / 2);
    vertices[1] = vec3f(position.x + size.x / 2, position.y - size.x / 2, position.z - size.x / 2);
    vertices[2] = vec3f(position.x - size.x / 2, position.y + size.x / 2, position.z - size.x / 2);
    vertices[3] = vec3f(position.x - size.x / 2, position.y - size.x / 2, position.z + size.x / 2);

    vertices[4] = vec3f(position.x + size.x / 2, position.y + size.x / 2, position.z - size.x / 2);
    vertices[5] = vec3f(position.x + size.x / 2, position.y - size.x / 2, position.z + size.x / 2);
    vertices[6] = vec3f(position.x - size.x / 2, position.y + size.x / 2, position.z + size.x / 2);
    vertices[7] = vec3f(position.x + size.x / 2, position.y + size.x / 2, position.z + size.x / 2);

    for (int i = 0; i < 8; i++)
        vertices[i] = rotation * vertices[i];
}

MathLib::AABB MathLib::OOBB::GetAABB() const
{
    vec3f min, max;
    vec3f points[8];

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

    return AABB(min, max);
}

bool MathLib::OOBB::IntersectsAABB(const AABB & /*bbox*/)
{
    throw;
}

bool MathLib::OOBB::IntersectsOOBB(const OOBB & bbox)
{
    vec3f a = this->size,
        b = bbox.size,
        Pa = this->position,
        Pb = bbox.position;

    mat3f A = this->rotation,
        B = bbox.rotation;

    //???????? ? ??????? ??????? ?????????
    vec3f v = Pb - Pa;

    //???????? ? ??????? ????????? ?
    vec3f T = A * v;

    //??????? ??????? ???????? B ???????????? ?
    const mat3f R(A * B.transpose());

    float ra, rb, t;

    //??????? ????????? ?
    for (int i = 0; i < 3; i++)
    {
        ra = a.el[i];
        rb = b.el[0] * fabs(R[i][0]) + b.el[1] * fabs(R[i][1]) + b.el[2] * fabs(R[i][2]);
        t = fabs(T.el[i]);
        if (t > ra + rb)
            return false;
    }

    //??????? ????????? B
    for (int k = 0; k < 3; k++)
    {
        ra = a.el[0] * fabs(R[0][k]) + a.el[1] * fabs(R[1][k]) + a.el[2] * fabs(R[2][k]);
        rb = b.el[k];
        t = fabs(T.el[0] * R[0][k] + T.el[1] * R[1][k] + T.el[2] * R[2][k]);
        if (t > ra + rb)
            return false;
    }

    //9 ????????? ????????????
    //L = A0 x B0
    ra = a.el[1] * fabs(R[2][0]) + a.el[2] * fabs(R[1][0]);
    rb = b.el[1] * fabs(R[0][2]) + b.el[2] * fabs(R[0][1]);
    t = fabs(T.el[2] * R[1][0] - T.el[1] * R[2][0]);
    if (t > ra + rb)
        return false;

    //L = A0 x B1
    ra = a.el[1] * fabs(R[2][1]) + a.el[2] * fabs(R[1][1]);
    rb = b.el[0] * fabs(R[0][2]) + b.el[2] * fabs(R[0][0]);
    t = fabs(T.el[2] * R[1][1] - T.el[1] * R[2][1]);
    if (t > ra + rb)
        return false;

    //L = A0 x B2
    ra = a.el[1] * fabs(R[2][2]) + a.el[2] * fabs(R[1][2]);
    rb = b.el[0] * fabs(R[0][1]) + b.el[1] * fabs(R[0][0]);
    t = fabs(T.el[2] * R[1][2] - T.el[1] * R[2][2]);
    if (t > ra + rb)
        return false;

    //L = A1 x B0
    ra = a.el[0] * fabs(R[2][0]) + a.el[2] * fabs(R[0][0]);
    rb = b.el[1] * fabs(R[1][2]) + b.el[2] * fabs(R[1][1]);
    t = fabs(T.el[0] * R[2][0] - T.el[2] * R[0][0]);
    if (t > ra + rb)
        return false;

    //L = A1 x B1
    ra = a.el[0] * fabs(R[2][1]) + a.el[2] * fabs(R[0][1]);
    rb = b.el[0] * fabs(R[1][2]) + b.el[2] * fabs(R[1][0]);
    t = fabs(T.el[0] * R[2][1] - T.el[2] * R[0][1]);
    if (t > ra + rb)
        return false;

    //L = A1 x B2
    ra = a.el[0] * fabs(R[2][2]) + a.el[2] * fabs(R[0][2]);
    rb = b.el[0] * fabs(R[1][1]) + b.el[1] * fabs(R[1][0]);
    t = fabs(T.el[0] * R[2][2] - T.el[2] * R[0][2]);
    if (t > ra + rb)
        return false;

    //L = A2 x B0
    ra = a.el[0] * fabs(R[1][0]) + a.el[1] * fabs(R[0][0]);
    rb = b.el[1] * fabs(R[2][2]) + b.el[2] * fabs(R[2][1]);
    t = fabs(T.el[1] * R[0][0] - T.el[0] * R[1][0]);
    if (t > ra + rb)
        return false;

    //L = A2 x B1
    ra = a.el[0] * fabs(R[1][1]) + a.el[1] * fabs(R[0][1]);
    rb = b.el[0] * fabs(R[2][2]) + b.el[2] * fabs(R[2][0]);
    t = fabs(T.el[1] * R[0][1] - T.el[0] * R[1][1]);
    if (t > ra + rb)
        return false;

    //L = A2 x B2
    ra = a.el[0] * fabs(R[1][2]) + a.el[1] * fabs(R[0][2]);
    rb = b.el[0] * fabs(R[2][1]) + b.el[1] * fabs(R[2][0]);
    t = fabs(T.el[1] * R[0][2] - T.el[0] * R[1][2]);
    if (t > ra + rb)
        return false;

    return true;
}

bool MathLib::OOBB::IntersectsSphere(const vec3f & /*center*/, float /*raduis*/)
{
    throw;
}

bool MathLib::OOBB::IntersectsLineSegment(const vec3f &mid, const vec3f &dir, const int hl)
{
    const vec3f tempSizes = this->size;
    const vec3f tempPos = this->position;

    vec3f newMid = this->rotation * mid;
    vec3f newDir = this->rotation * dir;

    const vec3f T = tempPos - newMid;

    float r;

    // ?????????, ???????? ?? ???? ?? ???? X,Y,Z ???????????
    if ((fabs(T.x) > tempSizes.x + hl*fabs(newDir.x)) ||
        (fabs(T.y) > tempSizes.y + hl*fabs(newDir.y)) ||
        (fabs(T.z) > tempSizes.z + hl*fabs(newDir.z)))
        return false;

    // ????????? X ^ dir
    r = tempSizes.y*fabs(newDir.z) + tempSizes.z*fabs(newDir.y);
    if (fabs(T.y*newDir.z - T.z*newDir.y) > r)
        return false;

    // ????????? Y ^ dir
    r = tempSizes.x*fabs(newDir.z) + tempSizes.z*fabs(newDir.x);
    if (fabs(T.z*newDir.x - T.x*newDir.z) > r)
        return false;

    // ????????? Z ^ dir
    r = tempSizes.x*fabs(newDir.y) + tempSizes.y*fabs(newDir.x);
    if (fabs(T.x*newDir.y - T.y*newDir.x) > r)
        return false;

    return true;
}

bool MathLib::OOBB::IntersectsTriangle(const vec3f &/*pnt1*/, const vec3f &/*pnt2*/, const vec3f &/*pnt3*/)
{
    throw;
}
