#pragma once

#include "Graphics/Color.hpp"
#include "BlendMode.hpp"

namespace pl
{

class VertexArray;
class Shader;
class Texture;

class RenderTarget
{
public:
    void clear(const Color& color);

    void draw(const VertexArray& vertexArray, const Shader& shader, const Texture* texture, BlendMode blendMode);

    virtual void bind() = 0;

    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
};

}