#pragma once

#include <cmath>

namespace MathLib
{

template<typename T>
struct vec2
{
    constexpr vec2()
        : x(0)
        , y(0)
    {
    }

    constexpr vec2(T _x, T _y)
        : x(_x)
        , y(_y)
    {
    }

    union
    {
        T el[2];
        struct
        {
            T x, y;
        };
    };

    constexpr T length() const
    {
        return (T)sqrt((double)x*x + (double)y*y);
    }

    constexpr const T * ptr() const
    {
        return el;
    }

    inline T * ptr()
    {
        return el;
    }

    inline vec2 & operator += (const vec2 & ref)
    {
        this->x += ref.x;
        this->y += ref.y;
        return *this;
    }

    inline vec2 & operator -= (const vec2 & ref)
    {
        this->x -= ref.x;
        this->y -= ref.y;
        return *this;
    }

    template<typename D>
    inline vec2 & operator *= (D num)
    {
        this->x *= (T)num;
        this->y *= (T)num;
        return *this;
    }

    template<typename T2>
    inline vec2 & operator = (vec2<T2> & vec)
    {
        this->x = (T)vec.x;
        this->y = (T)vec.y;
        return *this;
    }

    constexpr vec2 operator + (const vec2 & ref) const
    {
        return{ this->x + ref.x, this->y + ref.y };
    }

    constexpr vec2 operator - (const vec2 & ref) const
    {
        return{ this->x - ref.x, this->y - ref.y };
    }

    constexpr vec2 operator - () const
    {
        return{ -x, -y };
    }

    template<typename T2>
    constexpr operator vec2<T2>() const
    {
        return vec2<T2>((T2)x, (T2)y);
    }

    template<typename D>
    constexpr vec2 operator * (D num) const
    {
        return{ x * (T)num, y * (T)num };
    }

    constexpr bool operator == (const vec2 & ref) const
    {
        return ((this->x == ref.x) && (this->y == ref.y));
    }

    constexpr bool operator != (const vec2 & ref) const
    {
        return !((this->x == ref.x) && (this->y == ref.y));
    }
};

using vec2d = vec2<double>;
using vec2f = vec2<float>;
using vec2i = vec2<int>;

}
