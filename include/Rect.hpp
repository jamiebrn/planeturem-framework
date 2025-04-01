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