#pragma once

#include "vector2.h"

namespace MathLib
{

template<typename T>
struct line
{
    constexpr line() = default;

    constexpr line(T a, T b, T c)
        : a(a)
        , b(b)
        , c(c)
    {}

    constexpr bool operator==(const line& ref)
    {
        return a == ref.a
            && b == ref.b
            && c == ref.c;
    }

    constexpr bool operator!=(const line& ref)
    {
        return !operator==(ref);
    }

    T a = 0.0;
    T b = 0.0;
    T c = 0.0;
};

template<typename T>
plane<T> planeFromPointAndVector(const vec2<T>& point, const vec2<T>& vec)
{
}

template<typename T>
plane<T> planeFromTwoPoints(const vec2<T>& first, const vec2<T>& right)
{
}

}
