#pragma once

#include "ray3.h"
#include "plane.h"
#include "aabb2.h"
#include "aabb3.h"
#include "oobb3.h"

namespace MathLib
{

namespace intersections
{

// first - first box to check intersection with
// second - second box to check intersection with
template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> AABBsIntersection(const aabb3<T>& first, const aabb3<T>& second)
{
    return
        (first.bMin.x <= second.bMax.x) && (first.bMax.x >= second.bMin.x) &&
        (first.bMin.y <= second.bMax.y) && (first.bMax.y >= second.bMin.y) &&
        (first.bMin.z <= second.bMax.z) && (first.bMax.z >= second.bMin.z);
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> AABBsIntersection(const aabb2<T>& first, const aabb2<T>& second)
{
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> AABBSphereIntersection(const aabb2<T>& box, vec2<T> sphereCenter, T sphereRadius)
{
    return false;
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> AABBOOBBIntersection(const aabb2<T>& aabb, const oobb2<T>& oobb)
{
    return false;
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> AABBLineSegmentIntersection(const vec3<T> &mid, const vec3<T> &dir, const int hl)
{
    const vec3<T> sizes = { (bMax.x - bMin.x) / 2, (bMax.y - bMin.y) / 2, (bMax.z - bMin.z) / 2 };
    const vec3<T> position = { bMax.x - sizes.x, bMax.y - sizes.y, bMax.z - sizes.z };
    const vec3<T> p = position - mid;
    T r;

    if ((fabs(p.x) > sizes.x + hl*fabs(dir.x)) ||
        (fabs(p.y) > sizes.y + hl*fabs(dir.y)) ||
        (fabs(p.z) > sizes.z + hl*fabs(dir.z)))
        return false;

    r = sizes.y*fabs(dir.z) + sizes.z*fabs(dir.y);
    if (fabs(p.y*dir.z - p.z*dir.y) > r)
        return false;

    r = sizes.x*fabs(dir.z) + sizes.z*fabs(dir.x);
    if (fabs(p.z*dir.x - p.x*dir.z) > r)
        return false;

    r = sizes.x*fabs(dir.y) + sizes.y*fabs(dir.x);
    if (fabs(p.x*dir.y - p.y*dir.x) > r)
        return false;

    return true;
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> OOBBPointIntersection(const oobb2<T>& box, vec2<T> point)
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

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> OOBBSphereIntersection(const oobb2<T>& box, vec2<T> sphereCenter, T sphereRadius)
{
    matrix<T, 3> translationMatrix = matrixTranslation(center.x, center.y);
    matrix<T, 3> scalingMatrix = matrixScaling(sizes.x, sizes.y);
    matrix<T, 3> rotationMatrix = matrixRotationZ(angle);
    matrix<T, 3> total = scalingMatrix * rotationMatrix * translationMatrix;
    point = normalize(vec2Transform(sphereCenter, total));

    return false;
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> OOBBAABBIntersection(const oobb3<T>& oobb, const aabb3<T>& aabb)
{
    throw;
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> OOBBsIntesection(const oobb3<T>& first, const oobb3<T>& second)
{
    vec3<T> a = this->size;
    vec3<T> b = bbox.size;
    vec3<T> Pa = this->position;
    vec3<T> Pb = bbox.position;

    matrix<T, 3> A = this->rotation;
    matrix<T, 3> B = bbox.rotation;

    vec3<T> v = Pb - Pa;
    vec3<T> p = A * v;
    const mat3f R(A * B.transpose());

    float ra, rb, t;

    for (int i = 0; i < 3; i++)
    {
        ra = a.el[i];
        rb = b.el[0] * fabs(R[i][0]) + b.el[1] * fabs(R[i][1]) + b.el[2] * fabs(R[i][2]);
        t = fabs(p.el[i]);
        if (t > ra + rb)
            return false;
    }

    for (int k = 0; k < 3; k++)
    {
        ra = a.el[0] * fabs(R[0][k]) + a.el[1] * fabs(R[1][k]) + a.el[2] * fabs(R[2][k]);
        rb = b.el[k];
        t = fabs(p.el[0] * R[0][k] + p.el[1] * R[1][k] + p.el[2] * R[2][k]);
        if (t > ra + rb)
            return false;
    }

    ra = a.el[1] * fabs(R[2][0]) + a.el[2] * fabs(R[1][0]);
    rb = b.el[1] * fabs(R[0][2]) + b.el[2] * fabs(R[0][1]);
    t = fabs(p.el[2] * R[1][0] - p.el[1] * R[2][0]);
    if (t > ra + rb)
        return false;

    ra = a.el[1] * fabs(R[2][1]) + a.el[2] * fabs(R[1][1]);
    rb = b.el[0] * fabs(R[0][2]) + b.el[2] * fabs(R[0][0]);
    t = fabs(p.el[2] * R[1][1] - p.el[1] * R[2][1]);
    if (t > ra + rb)
        return false;

    ra = a.el[1] * fabs(R[2][2]) + a.el[2] * fabs(R[1][2]);
    rb = b.el[0] * fabs(R[0][1]) + b.el[1] * fabs(R[0][0]);
    t = fabs(p.el[2] * R[1][2] - p.el[1] * R[2][2]);
    if (t > ra + rb)
        return false;

    ra = a.el[0] * fabs(R[2][0]) + a.el[2] * fabs(R[0][0]);
    rb = b.el[1] * fabs(R[1][2]) + b.el[2] * fabs(R[1][1]);
    t = fabs(p.el[0] * R[2][0] - p.el[2] * R[0][0]);
    if (t > ra + rb)
        return false;

    ra = a.el[0] * fabs(R[2][1]) + a.el[2] * fabs(R[0][1]);
    rb = b.el[0] * fabs(R[1][2]) + b.el[2] * fabs(R[1][0]);
    t = fabs(p.el[0] * R[2][1] - p.el[2] * R[0][1]);
    if (t > ra + rb)
        return false;

    ra = a.el[0] * fabs(R[2][2]) + a.el[2] * fabs(R[0][2]);
    rb = b.el[0] * fabs(R[1][1]) + b.el[1] * fabs(R[1][0]);
    t = fabs(p.el[0] * R[2][2] - p.el[2] * R[0][2]);
    if (t > ra + rb)
        return false;

    ra = a.el[0] * fabs(R[1][0]) + a.el[1] * fabs(R[0][0]);
    rb = b.el[1] * fabs(R[2][2]) + b.el[2] * fabs(R[2][1]);
    t = fabs(p.el[1] * R[0][0] - p.el[0] * R[1][0]);
    if (t > ra + rb)
        return false;

    ra = a.el[0] * fabs(R[1][1]) + a.el[1] * fabs(R[0][1]);
    rb = b.el[0] * fabs(R[2][2]) + b.el[2] * fabs(R[2][0]);
    t = fabs(p.el[1] * R[0][1] - p.el[0] * R[1][1]);
    if (t > ra + rb)
        return false;

    ra = a.el[0] * fabs(R[1][2]) + a.el[1] * fabs(R[0][2]);
    rb = b.el[0] * fabs(R[2][1]) + b.el[1] * fabs(R[2][0]);
    t = fabs(p.el[1] * R[0][2] - p.el[0] * R[1][2]);
    if (t > ra + rb)
        return false;

    return true;
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> OOBBSphereIntersection(const oobb3<T>& box, const vec3<T> & sphereCenter, T sphereRadius)
{
    throw;
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> OOBBLineSegmentIntersection(const vec3<T> &mid, const vec3<T> &dir, const int hl)
{
    const vec3<T> tempSizes = this->size;
    const vec3<T> tempPos = this->position;

    vec3<T> newMid = this->rotation * mid;
    vec3<T> newDir = this->rotation * dir;

    const vec3<T> p = tempPos - newMid;

    float r;

    if ((fabs(p.x) > tempSizes.x + hl*fabs(newDir.x)) ||
        (fabs(p.y) > tempSizes.y + hl*fabs(newDir.y)) ||
        (fabs(p.z) > tempSizes.z + hl*fabs(newDir.z)))
        return false;

    r = tempSizes.y*fabs(newDir.z) + tempSizes.z*fabs(newDir.y);
    if (fabs(p.y*newDir.z - p.z*newDir.y) > r)
        return false;

    r = tempSizes.x*fabs(newDir.z) + tempSizes.z*fabs(newDir.x);
    if (fabs(p.z*newDir.x - p.x*newDir.z) > r)
        return false;

    r = tempSizes.x*fabs(newDir.y) + tempSizes.y*fabs(newDir.x);
    if (fabs(p.x*newDir.y - p.y*newDir.x) > r)
        return false;

    return true;
}

// ray - ray which check to intersection
// bbox - bounding box to check
// t0, t1 - start/end points on ray
template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> RayAABBIntersection(const ray3<T> & ray, const aabb3<T> & bbox, T t0, T t1)
{
    T tmin, tmax, tymin, tymax, tzmin, tzmax;

    T divx = 1.0 / ray.direction.x;
    if (divx >= 0.0)
    {
        tmin = (bbox.bMin.x - ray.origin.x) * divx;
        tmax = (bbox.bMax.x - ray.origin.x) * divx;
    }
    else
    {
        tmin = (bbox.bMax.x - ray.origin.x) * divx;
        tmax = (bbox.bMin.x - ray.origin.x) * divx;
    }

    T divy = 1.0 / ray.direction.y;
    if (divy >= 0.0)
    {
        tymin = (bbox.bMin.y - ray.origin.y) * divy;
        tymax = (bbox.bMax.y - ray.origin.y) * divy;
    }
    else
    {
        tymin = (bbox.bMax.y - ray.origin.y) * divy;
        tymax = (bbox.bMin.y - ray.origin.y) * divy;
    }

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    T divz = 1.0 / ray.direction.z;
    if (divz >= 0.0)
    {
        tzmin = (bbox.bMin.z - ray.origin.z) * divz;
        tzmax = (bbox.bMax.z - ray.origin.z) * divz;
    }
    else
    {
        tzmin = (bbox.bMax.z - ray.origin.z) * divz;
        tzmax = (bbox.bMin.z - ray.origin.z) * divz;
    }

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    return ((tmin < t1) && (tmax > t0));
}

// s1, s2 - centers of spheres to test
// r1, r2 - radiuses of spheres
template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> SphereSphereIntersection(vec3<T> s1, vec3<T> s2, T r1, T r2)
{
    return distance(s1, s2) <= r1 + r2;
}

// ray - ray which check to intersection
// s1 - sphere center
// r1 - sphere radius
template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> RaySphereIntersection(const ray3<T> & r, const vec3<T> & s1, T r1)
{
    vec3<T> diff = s1 - r.origin;

    if (MathLib::distance(s1, r.origin) > r1 && MathLib::dot(diff, r.direction) < 0.0f)
        return false;

    vec3<T> res = MathLib::cross(diff, r.direction);
    float distance = (res.length()) / (r.direction.length());
    if (distance < r1)
        return true;

    return false;
}

// t1, t2, t3 - triangle vertices
// l1,l2 - line start/end points
// p - intersection point
template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> LineTriangleIntersectionPoint(const vec3<T> &t1, const vec3<T> &t2, const vec3<T> &t3,
                                                                                       const vec3<T> &l1, const vec3<T> &l2, vec3<T> &p)
{
    // normal calculation
    vec3f n = normalize(cross(t2 - t1, t3 - t2));

    // distances to check
    float d1 = dot((l1 - t1), n) / n.length();
    float d2 = dot((l2 - t1), n) / n.length();

    // both points on the one side or line is parallel to triangle
    if (d1 * d2 > 0.0f || d1 == d2)
        return false;

    // intersection point
    p = l1 + (l2 - l1) * (-d1 / (d2 - d1));

    // test result point to check in lies inside triangle
    if (dot(cross(t2 - t1, p - t1), n) <= 0)
        return false;

    if (dot(cross(t3 - t2, p - t2), n) <= 0)
        return false;

    if (dot(cross(t1 - t3, p - t3), n) <= 0)
        return false;

    return true;
}

}
}
