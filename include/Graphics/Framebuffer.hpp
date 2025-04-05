#pragma once

#include <extlib/gl.h>
#include <cstdint>

#include "Graphics/RenderTarget.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Color.hpp"

namespace pl
{

class Framebuffer : public RenderTarget
{
public:
    Framebuffer() = default;
    ~Framebuffer();
    Framebuffer(const Framebuffer& Framebuffer) = delete;

    bool create(uint32_t width, uint32_t height);

    void setTextureRepeat(bool repeat);
    void setLinearFilter(bool value);

    const Texture& getTexture();

    virtual void bind() override;

    virtual int getWidth() override;
    virtual int getHeight() override;

private:
    GLuint framebuffer = 0;

    Texture texture;

};

}