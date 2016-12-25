#include "AABB.h"

MathLib::AABB::AABB(const vec3f & _bmin, const vec3f & _bmax)
    : bMin(_bmin)
    , bMax(_bmax)
{
}

void MathLib::AABB::GetVertices(vec3f vertices[8]) const
{
    vertices[0] = { bMin };
    vertices[1] = { bMax.x, bMin.y, bMin.z };
    vertices[2] = { bMin.x, bMax.y, bMin.z };
    vertices[3] = { bMax.x, bMax.y, bMin.z };

    vertices[4] = { bMin.x, bMin.y, bMax.z };
    vertices[5] = { bMin.x, bMax.y, bMax.z };
    vertices[6] = { bMax.x, bMin.y, bMax.z };
    vertices[7] = { bMax };
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
    bMin.x = std::min(bMin.x, otherAABBox.bMin.x);
    bMin.y = std::min(bMin.y, otherAABBox.bMin.y);
    bMin.z = std::min(bMin.z, otherAABBox.bMin.z);

    bMax.x = std::max(bMax.x, otherAABBox.bMax.x);
    bMax.y = std::max(bMax.y, otherAABBox.bMax.y);
    bMax.z = std::max(bMax.z, otherAABBox.bMax.z);
}

float MathLib::AABB::GetSurfaceArea() const
{
    float l = std::abs(bMax.x - bMin.x);
    float h = std::abs(bMax.y - bMin.y);
    float w = std::abs(bMax.z - bMin.z);

    return 2.0f * (l * w + l * h + w * h);
}

float MathLib::AABB::GetVolume() const
{
    float l = std::abs(bMax.x - bMin.x);
    float h = std::abs(bMax.y - bMin.y);
    float w = std::abs(bMax.z - bMin.z);

    return l * h * w;
}

bool MathLib::AABB::IntersectsLineSegment(const vec3f &mid, const vec3f &dir, const int hl) const
{
    const vec3f sizes = { (bMax.x - bMin.x) / 2, (bMax.y - bMin.y) / 2, (bMax.z - bMin.z) / 2 };
    const vec3f position = { bMax.x - sizes.x, bMax.y - sizes.y, bMax.z - sizes.z };
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
