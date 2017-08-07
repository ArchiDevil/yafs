#pragma once

#include "ray3.h"
#include "plane.h"
#include "aabb2.h"
#include "aabb3.h"
#include "oobb3.h"

#include <bitset>

namespace MathLib
{

namespace intersections
{

// first - first box to check intersection with
// second - second box to check intersection with
template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> AABBsIntersection(const aabb2<T>& first, const aabb2<T>& second)
{
    return std::abs(first.position.x - second.position.x) < first.sizes.x + second.sizes.x
        || std::abs(first.position.y - second.position.y) < first.sizes.y + second.sizes.y;
}

// first - first box to check intersection with
// second - second box to check intersection with
template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> AABBsIntersection(const aabb3<T>& first, const aabb3<T>& second)
{
    return (first.bMin.x <= second.bMax.x) && (first.bMax.x >= second.bMin.x)
        && (first.bMin.y <= second.bMax.y) && (first.bMax.y >= second.bMin.y)
        && (first.bMin.z <= second.bMax.z) && (first.bMax.z >= second.bMin.z);
}

template<typename T, template <typename> typename U, template <typename> typename V, size_t axisCount>
std::enable_if_t<std::is_floating_point<T>::value, bool> InternalAABBSphereIntersection(const U<T>& box, V<T> sphereCenter, T sphereRadius)
{
    T d = 0.0;

    for (int i = 0; i < axisCount; ++i)
    {
        T mina = box.position[i] - box.sizes[i];
        if (sphereCenter[i] < mina)
        {
            T a = sphereCenter[i] - mina;
            d += a * a;
        }

        T maxa = box.position[i] + box.sizes[i];
        if (sphereCenter[i] > maxa)
        {
            T a = sphereCenter[i] - maxa;
            d += a * a;
        }
    }

    return d <= (sphereRadius * sphereRadius);
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> AABBSphereIntersection(const aabb2<T>& box, vec2<T> sphereCenter, T sphereRadius)
{
    return InternalAABBSphereIntersection<T, aabb2, vec2, 2>(box, sphereCenter, sphereRadius);
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> AABBSphereIntersection(const aabb3<T>& box, vec3<T> sphereCenter, T sphereRadius)
{
    return InternalAABBSphereIntersection<T, aabb3, vec3, 3>(box, sphereCenter, sphereRadius);
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> AABBPointIntersection(const aabb2<T>& aabb, vec2<T> point)
{
    return point.x > aabb.position.x - aabb.sizes.x && point.x < aabb.position.x + aabb.sizes.x
        && point.y > aabb.position.y - aabb.sizes.y && point.y < aabb.position.y + aabb.sizes.y;
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> AABBOOBBIntersection(const aabb2<T>& aabb, const oobb2<T>& oobb)
{
    // first, check projection of OOBB on AABB coordinates
    matrix<T, 4> transformationMatrix = matrixRotationZ(oobb.angle) * matrixTranslation<T>(vec3<T>{oobb.position.x - aabb.position.x, oobb.position.y - aabb.position.y, 1.0f});
    vec2<T> points[4] = {
        { -oobb.sizes.x, -oobb.sizes.y },
        {  oobb.sizes.x, -oobb.sizes.y },
        { -oobb.sizes.x,  oobb.sizes.y },
        {  oobb.sizes.x,  oobb.sizes.y }
    };

    for (auto& point : points)
        point = vec2Transform(point, transformationMatrix);

    std::bitset<4> intersections{ 0 };

    for (auto& point : points)
    {
        if (point.x > -aabb.sizes.x &&
            point.x < aabb.sizes.x)
            intersections[0] = true;

        if (point.y > -aabb.sizes.y &&
            point.y < aabb.sizes.y)
            intersections[1] = true;

        if (intersections[0] && intersections[1])
            break;
    }

    if (intersections.count() < 2)
        return false;

    transformationMatrix = matrixTranslation<T>(vec3<T>{aabb.position.x - oobb.position.x, aabb.position.y - oobb.position.y, 1.0f}) * matrixRotationZ(-oobb.angle);
    points[0] = { -aabb.sizes.x, -aabb.sizes.y };
    points[1] = {  aabb.sizes.x, -aabb.sizes.y };
    points[2] = { -aabb.sizes.x,  aabb.sizes.y };
    points[3] = {  aabb.sizes.x,  aabb.sizes.y };

    for (auto& point : points)
        point = vec2Transform(point, transformationMatrix);

    for (auto& point : points)
    {
        if (point.x > -oobb.sizes.x &&
            point.x < oobb.sizes.x)
            intersections[2] = true;

        if (point.y > -oobb.sizes.y &&
            point.y < oobb.sizes.y)
            intersections[3] = true;

        if (intersections[2] && intersections[3])
            break;
    }

    return intersections.all();
}

//template<typename T>
//std::enable_if_t<std::is_floating_point<T>::value, bool> AABBOOBBIntersection(const oobb3<T>& oobb, const aabb3<T>& aabb)
//{
//    throw std::runtime_error("Not implemented yet!");
//}

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
    matrix<T, 4> translationMatrix = matrixTranslation(box.position.x, box.position.y);
    matrix<T, 4> scalingMatrix = matrixScaling(box.sizes.x, box.sizes.y);
    matrix<T, 4> rotationMatrix = matrixRotationZ(box.angle);
    matrix<T, 4> total = scalingMatrix * rotationMatrix * translationMatrix;
    point = vec2Transform(point, total);

    return (point.x >= -0.5f || point.x <= 0.5f)
        && (point.y >= -0.5f || point.y <= 0.5f);
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> OOBBsIntersection(const oobb2<T>& first, const oobb2<T>& second)
{
    throw std::runtime_error("Not implemented yet!");
}

//template<typename T>
//std::enable_if_t<std::is_floating_point<T>::value, bool> OOBBsIntersection(const oobb3<T>& first, const oobb3<T>& second)
//{
//    throw std::runtime_error("Not implemented yet!");
//}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> OOBBSphereIntersection(const oobb2<T>& box, const vec2<T>& sphereCenter, T sphereRadius)
{
    matrix<T, 4> rotationMatrix = matrixRotationZ(box.angle);
    auto transformedCenter = vec2Transform(sphereCenter, rotationMatrix);
    return AABBSphereIntersection({ box.position, box.sizes }, transformedCenter, sphereRadius);
}

//template<typename T>
//std::enable_if_t<std::is_floating_point<T>::value, bool> OOBBSphereIntersection(const oobb3<T>& box, const vec3<T>& sphereCenter, T sphereRadius)
//{
//    throw std::runtime_error("Not implemented yet!");
//}

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

    T divx = T{ 1.0 } / ray.direction.x;
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

    T divy = T{ 1.0 } / ray.direction.y;
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

    T divz = T{ 1.0 } / ray.direction.z;
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
std::enable_if_t<std::is_floating_point<T>::value, bool> SphereSphereIntersection(vec2<T> s1, vec2<T> s2, T r1, T r2)
{
    return distance(s1, s2) <= r1 + r2;
}

// s1, s2 - centers of spheres to test
// r1, r2 - radiuses of spheres
template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> SphereSphereIntersection(vec3<T> s1, vec3<T> s2, T r1, T r2)
{
    return distance(s1, s2) <= r1 + r2;
}

// ray - ray which check to intersection
// sphereCenter - sphere center
// sphereRadius - sphere radius
template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, bool> RaySphereIntersection(const ray3<T> & ray, const vec3<T> & sphereCenter, T sphereRadius)
{
    vec3<T> diff = sphereCenter - ray.origin;

    if (MathLib::distance(sphereCenter, ray.origin) > sphereRadius &&
        MathLib::dot(diff, ray.direction) < 0.0f)
        return false;

    vec3<T> res = MathLib::cross(diff, ray.direction);
    float distance = (res.length()) / (ray.direction.length());
    if (distance < sphereRadius)
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
