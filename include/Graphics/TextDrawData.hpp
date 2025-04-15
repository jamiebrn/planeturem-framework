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
    unsigned int outlineThickness = 0;

    bool centeredX = false;
    bool centeredY = false;

    bool containOnScreenX = false;
    bool containOnScreenY = false;
    int containPaddingLeft = 0;
    int containPaddingRight = 0;
    int containPaddingTop = 0;
    int containPaddingBottom = 0;
};

}