#pragma once

#include "vectorsFuncs.h"

namespace MathLib
{

template<typename T>
struct plane
{
    constexpr plane() = default;

    constexpr plane(T a, T b, T c, T d)
        : a(a)
        , b(b)
        , c(c)
        , d(d)
    {
    }

    constexpr bool operator == (const plane & ref) const
    {
        return (a == ref.a) && (b == ref.b) && (c == ref.c) && (d == ref.d);
    }

    constexpr bool operator != (const plane & ref) const
    {
        return (*this == ref);
    }

    T a = 0.0, b = 0.0, c = 0.0, d = 0.0;
};

template<typename T>
plane<T> normalize(const plane<T> & p)
{
    T inv_length = (T)1.0 / vec3<T>(p.a, p.b, p.c).length();
    return{ p.a * inv_length, p.b * inv_length, p.c * inv_length, p.d * inv_length };
}

template<typename T>
constexpr T planeDotNormal(const plane<T> & p, const vec3<T> & v)
{
    return p.a * v.x + p.b * v.y + p.c * v.z + p.d * (T)0.0;
}

template<typename T>
constexpr T planeDotCoord(const plane<T> & p, const vec3<T> & v)
{
    return p.a * v.x + p.b * v.y + p.c * v.z + p.d * (T)1.0;
}

}
