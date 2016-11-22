#pragma once

#include <cmath>

namespace MathLib
{

template<typename T>
struct vec4
{
    constexpr vec4()
        : x(0)
        , y(0)
        , z(0)
        , w(0)
    {
    }

    constexpr vec4(T _x, T _y, T _z, T _w)
        : x(_x)
        , y(_y)
        , z(_z)
        , w(_w)
    {
    }

    union
    {
        T el[4];
        struct
        {
            T x, y, z, w;
        };
    };

    constexpr T length() const
    {
        return (T)sqrt((double)x*x + (double)y*y + (double)z*z + (double)w*w);
    }

    constexpr const T * ptr() const
    {
        return el;
    }

    inline T * ptr()
    {
        return el;
    }

    inline vec4 & operator += (const vec4 & ref)
    {
        this->x += ref.x;
        this->y += ref.y;
        this->z += ref.z;
        this->w += ref.w;
        return *this;
    }

    inline vec4 & operator -= (const vec4 & ref)
    {
        this->x -= ref.x;
        this->y -= ref.y;
        this->z -= ref.z;
        this->w -= ref.w;
        return *this;
    }

    template<typename D>
    inline vec4 & operator *= (D num)
    {
        this->x *= (T)num;
        this->y *= (T)num;
        this->z *= (T)num;
        this->w *= (T)num;
        return *this;
    }

    inline vec4 & operator = (const vec4 & vec)
    {
        this->x = vec.x;
        this->y = vec.y;
        this->z = vec.z;
        this->w = vec.w;
        return *this;
    }

    template<typename T2> 
    constexpr operator vec4<T2>() const
    {
        return vec4<T2>((T2)x, (T2)y, (T2)z, (T2)w);
    }

    constexpr vec4 operator + (const vec4 & ref) const
    {
        return{ this->x + ref.x, this->y + ref.y, this->z + ref.z, this->w + ref.w };
    }

    constexpr vec4 operator - (const vec4 & ref) const
    {
        return{ this->x - ref.x, this->y - ref.y, this->z - ref.z, this->w - ref.w };
    }

    constexpr vec4 operator - () const
    {
        return{ -x, -y, -z, -w };
    }

    template<typename D>
    constexpr vec4 operator * (D num) const
    {
        return{ x * (T)num, y * (T)num, z * (T)num, w * (T)num };
    }

    constexpr bool operator == (const vec4 & ref) const
    {
        return ((this->x == ref.x) && (this->y == ref.y) && (this->z == ref.z) && (this->w == ref.w));
    }

    constexpr bool operator != (const vec4 & ref) const
    {
        return !((this->x == ref.x) && (this->y == ref.y) && (this->z == ref.z) && (this->w == ref.w));
    }
};

using vec4f = vec4<float>;
using vec4d = vec4<double>;
using vec4i = vec4<int>;

}
