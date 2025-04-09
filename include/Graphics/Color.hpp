#pragma once

namespace pl
{

struct Color
{
    float r;
    float g;
    float b;
    float a;

    Color(float r = 255.0f, float g = 255.0f, float b = 255.0f, float a = 255.0f) : r(r), g(g), b(b), a(a) {}

    inline Color normalise() const
    {
        return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }
    
    inline bool operator==(const Color& color) const
    {
        return (r == color.r && g == color.g && b == color.b && a == color.a);
    }
};

}