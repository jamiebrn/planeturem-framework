#pragma once

#include <string>

#include "Vector.hpp"
#include "Graphics/Color.hpp"

namespace pl
{

struct TextDrawData
{
    std::string text;
    Vector2f position;
    Color color;
    unsigned int size;

    Color outlineColor = Color(0, 0, 0);
    float outlineThickness = 0;

    bool centeredX = false;
    bool centeredY = false;

    bool smoothing = false;

    bool containOnScreenX = false;
    bool containOnScreenY = false;
    float containPaddingLeft = 0;
    float containPaddingRight = 0;
    float containPaddingTop = 0;
    float containPaddingBottom = 0;
};

}