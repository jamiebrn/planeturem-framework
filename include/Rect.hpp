#pragma once

#include "Vector.hpp"

namespace pl
{

template <class T>
struct Rect
{
    T x;
    T y;
    T width;
    T height;

    Rect() : x(0), y(0), width(0), height(0);
    Rect(T x, T y, T width, T height) : x(x), y(y), width(width), height(height) {}
    Rect(Vector2<T> position, Vector2<T> size) : x(position.x), y(position.y), width(size.x), height(size.y) {}

    inline Vector2<T> getPosition() const
    {
        return Vector2<T>(x, y);
    }

    inline Vector2<T> getSize() const
    {
        return Vector2<T>(width, height);
    }

    template <class U>
    inline operator Rect<U>() const
    {
        return Rect<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(width), static_cast<U>(height));
    }
};

}