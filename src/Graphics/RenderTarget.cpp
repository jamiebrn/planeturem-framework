#include "Graphics/RenderTarget.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/VertexArray.hpp"

int pl::RenderTarget::activeFrameBuffer = -1;
int pl::RenderTarget::currentBlendMode = -1;

void pl::RenderTarget::clear(const Color& color)
{
    bind();
    Color colorNormalised = color.normalise();
    glClearColor(colorNormalised.r, colorNormalised.g, colorNormalised.b, colorNormalised.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void pl::RenderTarget::draw(VertexArray& vertexArray, Shader& shader, const Texture* texture, BlendMode blendMode)
{
    bind();
    shader.bind();
    
    float halfTargetWidth = getWidth() / 2.0f;
    float halfTargetHeight = getHeight() / 2.0f;

    shader.setUniform2f("v_targetHalfSize", halfTargetWidth, halfTargetHeight);

    if (texture)
    {
        texture->use();
        shader.setUniform2f("v_textureSize", texture->getWidth(), texture->getHeight());
    }
    else
    {
        shader.setUniform2f("v_textureSize", 0, 0);
    }

    if (blendMode != currentBlendMode)
    {
        switch (blendMode)
        {
            case BlendMode::None:
            {
                glBlendFunc(GL_ONE, GL_ZERO);
                break;
            }
            case BlendMode::Add:
            {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            }
            case BlendMode::Multiply:
            {
                glBlendFunc(GL_DST_COLOR, GL_ZERO);
                break;
            }
            case BlendMode::Alpha:
            {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;
            }
        }

        currentBlendMode = blendMode;
    }

    vertexArray.draw(*this, texture);
}