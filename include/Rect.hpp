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

    Rect() = default;
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
};

}