#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

namespace pl
{

template <class T>
struct Vector2
{
    T x;
    T y;

    Vector2() = default;
    Vector2(T x, T y) : x(x), y(y) {}

    inline Vector2<T> rotate(float radians) const
    {
        Vector2<T> rotatedVector;
        rotatedVector.x = x * std::cos(radians) - y * std::sin(radians);
        rotatedVector.y = x * std::sin(radians) + y * std::cos(radians);

        return rotatedVector;
    }

    inline float getLengthSq() const
    {
        return x * x + y * y;
    }

    inline float getLength() const
    {
        return std::sqrt(getLengthSq());
    }

    inline Vector2<T> normalise() const
    {
        float length = getLength();
        if (length > 0)
        {
            return *this / length;
        }
        return Vector2<T>(0, 0);
    }

    inline Vector2<T> operator+(const Vector2<T>& vector) const
    {
        return Vector2<T>(x + vector.x, y + vector.y);
    }

    inline Vector2<T> operator-(const Vector2<T>& vector) const
    {
        return Vector2<T>(x - vector.x, y - vector.y);
    }

    inline Vector2<T> operator*(T mult) const
    {
        return Vector2<T>(x * mult, y * mult);
    }

    inline Vector2<T> operator/(T div) const
    {
        return Vector2<T>(x / div, y / div);
    }

    inline void operator+=(const Vector2<T>& vector) const
    {
        *this = *this + vector;
    }

    inline void operator-=(const Vector2<T>& vector) const
    {
        *this = *this - vector;
    }

    inline void operator*=(T mult) const
    {
        *this = *this * mult;
    }

    inline void operator/=(T div) const
    {
        *this = *this / div;
    }

    template <class U>
    inline operator Vector2<U>() const
    {
        return Vector2<U>(static_cast<U>(x), static_cast<U>(y));
    }
};

typedef Vector2<float> Vector2f;

template <class T>
struct Vector3
{
    T x;
    T y;
    T z;

    Vector3() = default;
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
};

template <class T>
struct Vector4
{
    T x;
    T y;
    T z;
    T w;

    Vector4() = default;
    Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
};

}