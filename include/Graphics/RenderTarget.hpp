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
    void draw(VertexArray& vertexArray, Shader& shader, GLuint texture, int textureWidth, int textureHeight, BlendMode blendMode);

    virtual void bind() = 0;

    virtual int getWidth() = 0;
    virtual int getHeight() = 0;

protected:
    static int activeFrameBuffer;

private:
    void draw(VertexArray& vertexArray, Shader& shader, BlendMode blendMode);

    static int currentBlendMode;

};

}