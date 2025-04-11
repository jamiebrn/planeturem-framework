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

    Vector2() : x(0), y(0) {}
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

    inline Vector2<T> operator+() const
    {
        return *this;
    }

    inline Vector2<T> operator-() const
    {
        return Vector2<T>(-x, -y);
    }

    inline Vector2<T> operator*(T mult) const
    {
        return Vector2<T>(x * mult, y * mult);
    }

    inline Vector2<T> operator/(T div) const
    {
        return Vector2<T>(x / div, y / div);
    }

    inline Vector2<T>& operator+=(const Vector2<T>& vector)
    {
        *this = *this + vector;
        return *this;
    }

    inline Vector2<T>& operator-=(const Vector2<T>& vector)
    {
        *this = *this - vector;
        return *this;
    }

    inline Vector2<T>& operator*=(T mult)
    {
        *this = *this * mult;
        return *this;
    }

    inline Vector2<T>& operator/=(T div)
    {
        *this = *this / div;
        return *this;
    }

    inline bool operator==(const Vector2<T>& vector) const
    {
        return (x == vector.x && y == vector.y);
    }

    template <class U>
    inline operator Vector2<U>() const
    {
        return Vector2<U>(static_cast<U>(x), static_cast<U>(y));
    }
};

template <class T>
inline Vector2<T> operator*(T mult, const Vector2<T>& vector)
{
    return vector * mult;
}

typedef Vector2<float> Vector2f;

template <class T>
struct Vector3
{
    T x;
    T y;
    T z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
};

template <class T>
struct Vector4
{
    T x;
    T y;
    T z;
    T w;

    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
};

}