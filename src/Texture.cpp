#include "Texture.hpp"

Texture::~Texture()
{
    if (textureId == 0)
    {
        return;
    }

    // Free memory
    glDeleteTextures(1, &textureId);
    textureId = 0;
}

bool Texture::loadTexture(const std::string& texturePath, bool mipmap)
{
    if (textureId == 0)
    {
        glGenTextures(1, &textureId);
    }

    stbi_set_flip_vertically_on_load(true);

    Image image;

    if (!image.loadFromFile(texturePath))
    {
        std::cout << "ERROR: Failed to load texture:\n" << SDL_GetError() << "\n";
        return false;
    }

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

void Texture::setTextureRepeat(bool repeat)
{
    if (textureId == 0)
    {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, textureId);

    if (repeat)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setLinearFilter(bool value)
{
    if (textureId == 0)
    {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, textureId);

    if (value)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::use() const
{
    glBindTexture(GL_TEXTURE_2D, textureId);
}

GLuint Texture::getID()
{
    return textureId;
}