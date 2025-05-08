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

    void draw(VertexArray& vertexArray, Shader& shader, const Texture* texture, BlendMode blendMode);

    virtual void bind() = 0;

    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;

protected:
    static int activeFrameBuffer;

private:
    static int currentBlendMode;

};

}