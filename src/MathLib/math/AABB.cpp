#include "AABB.h"

using std::min;
using std::abs;
using std::max;

MathLib::AABB::AABB(const vec3f & _bmin, const vec3f & _bmax)
    : bMin(_bmin)
    , bMax(_bmax)
{
}

void MathLib::AABB::GetVertices(vec3f vertices[8]) const
{
    vertices[0] = vec3f(bMin);
    vertices[1] = vec3f(bMax.x, bMin.y, bMin.z);
    vertices[2] = vec3f(bMin.x, bMax.y, bMin.z);
    vertices[3] = vec3f(bMax.x, bMax.y, bMin.z);

    vertices[4] = vec3f(bMin.x, bMin.y, bMax.z);
    vertices[5] = vec3f(bMin.x, bMax.y, bMax.z);
    vertices[6] = vec3f(bMax.x, bMin.y, bMax.z);
    vertices[7] = vec3f(bMax);
}

bool MathLib::AABB::IntersectsAABB(const AABB & otherAABBox) const
{
    return
        (bMin.x <= otherAABBox.bMax.x) && (bMax.x >= otherAABBox.bMin.x) &&
        (bMin.y <= otherAABBox.bMax.y) && (bMax.y >= otherAABBox.bMin.y) &&
        (bMin.z <= otherAABBox.bMax.z) && (bMax.z >= otherAABBox.bMin.z);
}

void MathLib::AABB::Include(const AABB & otherAABBox)
{
    bMin.x = min(bMin.x, otherAABBox.bMin.x);
    bMin.y = min(bMin.y, otherAABBox.bMin.y);
    bMin.z = min(bMin.z, otherAABBox.bMin.z);

    bMax.x = max(bMax.x, otherAABBox.bMax.x);
    bMax.y = max(bMax.y, otherAABBox.bMax.y);
    bMax.z = max(bMax.z, otherAABBox.bMax.z);
}

float MathLib::AABB::GetSurfaceArea() const
{
    float l = abs(bMax.x - bMin.x);
    float h = abs(bMax.y - bMin.y);
    float w = abs(bMax.z - bMin.z);

    return 2.0f * (l * w + l * h + w * h);
}

float MathLib::AABB::GetVolume() const
{
    float l = abs(bMax.x - bMin.x);
    float h = abs(bMax.y - bMin.y);
    float w = abs(bMax.z - bMin.z);

    return l * h * w;
}

bool MathLib::AABB::IntersectsLineSegment(const vec3f &mid, const vec3f &dir, const int hl) const
{
    const vec3f sizes = vec3f((bMax.x - bMin.x) / 2, (bMax.y - bMin.y) / 2, (bMax.z - bMin.z) / 2);
    const vec3f position = vec3f(bMax.x - sizes.x, bMax.y - sizes.y, bMax.z - sizes.z);
    const vec3f T = position - mid;
    float r;

    // проверяем, является ли одна из осей X,Y,Z разделяющей
    if ((fabs(T.x) > sizes.x + hl*fabs(dir.x)) ||
        (fabs(T.y) > sizes.y + hl*fabs(dir.y)) ||
        (fabs(T.z) > sizes.z + hl*fabs(dir.z)))
        return false;

    // проверяем X ^ dir
    r = sizes.y*fabs(dir.z) + sizes.z*fabs(dir.y);
    if (fabs(T.y*dir.z - T.z*dir.y) > r)
        return false;

    // проверяем Y ^ dir
    r = sizes.x*fabs(dir.z) + sizes.z*fabs(dir.x);
    if (fabs(T.z*dir.x - T.x*dir.z) > r)
        return false;

    // проверяем Z ^ dir
    r = sizes.x*fabs(dir.y) + sizes.y*fabs(dir.x);
    if (fabs(T.x*dir.y - T.y*dir.x) > r)
        return false;

    return true;
}

MathLib::vec3f MathLib::AABB::GetCentralPoint() const
{
    return vec3f((bMax.x - bMin.x) / 2.0f,
        (bMax.y - bMin.y) / 2.0f,
        (bMax.z - bMin.z) / 2.0f) + bMin;
}
