#include "Graphics/Framebuffer.hpp"

pl::Framebuffer::~Framebuffer()
{
    if (framebuffer > 0)
    {
        glDeleteFramebuffers(1, &framebuffer);
    }
}

bool pl::Framebuffer::create(uint32_t width, uint32_t height)
{
    if (framebuffer > 0)
    {
        if (getWidth() == width && getHeight() == height)
        {
            return true;
        }
        glDeleteFramebuffers(1, &framebuffer);
    }

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    GLuint textureColorBuffer;
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        return false;
    }

    texture.setFromAllocated(textureColorBuffer, width, height);

    return true;
}

void pl::Framebuffer::setTextureRepeat(bool repeat)
{
    texture.setTextureRepeat(repeat);
}

void pl::Framebuffer::setLinearFilter(bool value)
{
    texture.setLinearFilter(value);
}

const pl::Texture& pl::Framebuffer::getTexture()
{
    return texture;
}

void pl::Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, texture.getWidth(), texture.getHeight());
}

int pl::Framebuffer::getWidth()
{
    return texture.getWidth();
}

int pl::Framebuffer::getHeight()
{
    return texture.getHeight();
}