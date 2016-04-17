#pragma once

#include "ray.h"
#include "plane.h"
#include "AABB.h"
#include "OOBB.h"

namespace MathLib
{

bool RayBoxIntersect(const Ray & ray, const AABB & bbox, float t0, float t1);
bool SphereSphereIntersect(vec3f s1, vec3f s2, float r1, float r2);
bool RaySphereIntersect(const Ray & r, const vec3f & s1, float r1);
bool LineTriangleIntersectionPoint(const vec3f &t1, const vec3f &t2, const vec3f &t3, const vec3f &l1, const vec3f &l2, vec3f &p);

}
