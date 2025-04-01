#pragma once

#include "Vector.hpp"
#include "Color.hpp"

struct Vertex
{
    pl::Vector2f position;
    pl::Color color;
    pl::Vector2f textureUV;
};