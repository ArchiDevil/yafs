#pragma once

#include <cmath>

namespace MathLib
{

template<typename T>
struct vec3
{
    constexpr vec3()
        : x(0)
        , y(0)
        , z(0)
    {
    }

    constexpr vec3(T _x, T _y, T _z)
        : x(_x)
        , y(_y)
        , z(_z)
    {
    }

    union
    {
        T el[3];
        struct
        {
            T x, y, z;
        };
    };

    constexpr T length() const
    {
        return (T)sqrt((double)x*x + (double)y*y + (double)z*z);
    }

    inline T * ptr()
    {
        return el;
    }

    constexpr const T * ptr() const
    {
        return el;
    }

    inline vec3 & operator += (const vec3 & ref)
    {
        this->x += ref.x;
        this->y += ref.y;
        this->z += ref.z;
        return *this;
    }

    inline vec3 & operator -= (const vec3 & ref)
    {
        this->x -= ref.x;
        this->y -= ref.y;
        this->z -= ref.z;
        return *this;
    }

    template<typename D>
    inline vec3 & operator *= (D num)
    {
        this->x *= (T)num;
        this->y *= (T)num;
        this->z *= (T)num;
        return *this;
    }

    template<typename T2>
    inline vec3 & operator = (const vec3<T2> & vec)
    {
        this->x = (T)vec.x;
        this->y = (T)vec.y;
        this->z = (T)vec.z;
        return *this;
    }

    template<typename T2>
    constexpr operator vec3<T2>() const
    {
        return vec3<T2>((T2)x, (T2)y, (T2)z);
    }

    constexpr vec3 operator + (const vec3 & ref) const
    {
        return{ this->x + ref.x, this->y + ref.y, this->z + ref.z };
    }

    constexpr vec3 operator - (const vec3 & ref) const
    {
        return{ this->x - ref.x, this->y - ref.y, this->z - ref.z };
    }

    constexpr vec3 operator - () const
    {
        return{ -x, -y, -z };
    }

    template<typename D>
    constexpr vec3 operator * (D num) const
    {
        return{ x * (T)num, y * (T)num, z * (T)num };
    }

    constexpr bool operator == (const vec3 & ref) const
    {
        return ((this->x == ref.x) && (this->y == ref.y) && (this->z == ref.z));
    }

    constexpr bool operator != (const vec3 & ref) const
    {
        return !((this->x == ref.x) && (this->y == ref.y) && (this->z == ref.z));
    }
};

using vec3f = vec3<float>;
using vec3d = vec3<double>;
using vec3i = vec3<int>;

}
