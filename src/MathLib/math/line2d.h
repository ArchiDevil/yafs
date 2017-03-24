#pragma once

#include "vector2.h"

#include <type_traits>

namespace MathLib
{

template<typename T>
struct line2d
{
    constexpr line2d() = default;

    constexpr line2d(T a, T b, T c)
        : a(a)
        , b(b)
        , c(c)
    {}

    constexpr bool operator==(const line2d& ref) const
    {
        return a == ref.a
            && b == ref.b
            && c == ref.c;
    }

    constexpr bool operator!=(const line2d& ref) const
    {
        return !operator==(ref);
    }

    constexpr vec2<T> direction() const
    {
        return{ -b, a };
    }

    constexpr vec2<T> normal() const
    {
        return{ a, b };
    }

    std::enable_if_t<std::is_floating_point<T>::value, bool> proportional(const line2d& ref) const
    {
        T lambda = a / ref.a;
        return ref.b * lambda == b
            && ref.c * lambda == c;
    }

    std::enable_if_t<std::is_floating_point<T>::value, bool> parallel(const line2d& ref) const
    {
        T lambda = a / ref.a;
        return ref.b * lambda == b
            && ref.c * lambda != c;
    }

    std::enable_if_t<std::is_floating_point<T>::value, bool> intersects(const line2d& ref) const
    {
        T lambda = a / ref.a;
        return ref.b * lambda != b;
    }

    T a = 0.0;
    T b = 0.0;
    T c = 0.0;
};

template<typename T>
line2d<T> lineFromPointAndNormalVector(const vec2<T>& p, const vec2<T>& n)
{
    T d1 = n.x * p.x;
    T d2 = n.y * p.y;

    T a = n.x;
    T b = n.y;
    T c = -d1 - d2;

    if (a >= 0)
        return{ a, b, c };
    else
        return{ -a, -b, -c };
}

template<typename T>
line2d<T> lineFromTwoPoints(const vec2<T>& p1, const vec2<T>& p2)
{
    T d1 = p2.x - p1.x;
    T d2 = p2.y - p1.y;

    T a = d2;
    T b = -d1;
    T c = -d2 * p1.x + d1 * p1.y;

    if (a >= 0.0)
        return{ a, b, c };
    else
        return{ -a, -b, -c };
}

template<typename T>
line2d<T> lineFromPointAndDirectionVector(const vec2<T>& p, const vec2<T>& v)
{
    return lineFromTwoPoints(p, p + v);
}

template<typename T>
constexpr T distanceFromPointToLine(const line2d<T>& l, const vec2<T>& p)
{
    return abs(l.a * p.x + l.b * p.y + l.c) / sqrt(l.a * l.a + l.b * l.b);
}

template<typename T>
constexpr T angleBetweenTwoLines(const line2d<T>& l1, const line2d<T>& l2)
{
    return angle(l1.direction(), l2.direction());
}

template<typename T>
vec2<T> projectLineOnPoint(const vec2<T>& point, const line2d<T>& line)
{
    auto c2 = line.a * point.y - line.b * point.x;

    vec2<T> res;
    res.y = -(line.c * line.b - line.a * c2) / (line.b * line.b + line.a * line.a);
    res.x = -(line.b * res.y + line.c) / line.a;
    return res;
}

// Check if projected point lies between two points on line segment
template<typename T>
bool isPointInsideLineSegment(const vec2<T>& point, const vec2<T>& p1, const vec2<T>&p2)
{
    T distance1 = distance(p1, point);
    T distance2 = distance(p2, point);

    T segmentSize = distance(p1, p2);

    return distance1 <= segmentSize && distance2 <= segmentSize;
}

}
