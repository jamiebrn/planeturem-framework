#pragma once

#include "Vector.hpp"
#include "Color.hpp"

namespace pl
{

struct Vertex
{
    Vector2f position;
    Color color;
    Vector2f textureUV;

    Vertex() = default;
    Vertex(Vector2f position) : position(position) {}
    Vertex(Vector2f position, Color color) : position(position), color(color) {}
    Vertex(Vector2f position, Vector2f textureUV) : position(position), textureUV(textureUV) {}
    Vertex(Vector2f position, Color color, Vector2f textureUV) : position(position), color(color), textureUV(textureUV) {}
};

}