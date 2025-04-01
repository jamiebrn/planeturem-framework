#include "Texture.hpp"

pl::Texture::~Texture()
{
    if (textureId == 0)
    {
        return;
    }

    // Free memory
    glDeleteTextures(1, &textureId);
    textureId = 0;
}

bool pl::Texture::loadTexture(const std::string& texturePath, bool mipmap)
{
    if (textureId == 0)
    {
        glGenTextures(1, &textureId);
    }

    Image image;

    if (!image.loadFromFile(texturePath))
    {
        std::cout << "ERROR: Failed to load texture: " << texturePath << "\n";
        return false;
    }

    width = image.getWidth();
    height = image.getHeight();

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (mipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void pl::Texture::setTextureRepeat(bool repeat)
{
    if (textureId == 0)
    {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void pl::Texture::setLinearFilter(bool value)
{
    if (textureId == 0)
    {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, value ? GL_LINEAR : GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}

int pl::Texture::getWidth() const
{
    return width;
}

int pl::Texture::getHeight() const
{
    return height;
}

void pl::Texture::use() const
{
    glBindTexture(GL_TEXTURE_2D, textureId);
}

GLuint pl::Texture::getID()
{
    return textureId;
}