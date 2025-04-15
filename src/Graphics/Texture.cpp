#include "Graphics/Texture.hpp"

std::unordered_map<uint8_t, GLuint> pl::Texture::activeTextures;

pl::Texture::~Texture()
{
    if (textureId == 0)
    {
        return;
    }

    for (auto& unit : activeTextures)
    {
        if (unit.second == textureId)
        {
            unit.second = 0;
        }
    }

    // Free memory
    glDeleteTextures(1, &textureId);
    textureId = 0;
}

bool pl::Texture::loadTexture(const std::string& texturePath, bool mipmap)
{
    Image image;

    if (!image.loadFromFile(texturePath))
    {
        std::cout << "ERROR: Failed to load texture: " << texturePath << "\n";
        return false;
    }

    return loadTexture(image.getPixelPtr(), image.getWidth(), image.getHeight(), mipmap);
}

bool pl::Texture::loadTexture(uint8_t* pixels, int width, int height, bool mipmap)
{
    this->width = width;
    this->height = height;

    if (textureId == 0)
    {
        glGenTextures(1, &textureId);
    }

    bindTextureID(textureId, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (mipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    return true;
}

void pl::Texture::setFromAllocated(GLuint textureId, int width, int height)
{
    if (textureId != 0)
    {
        glDeleteTextures(1, &this->textureId);
    }

    this->textureId = textureId;

    this->width = width;
    this->height = height;
}

void pl::Texture::setTextureRepeat(bool repeat)
{
    if (textureId == 0)
    {
        return;
    }

    bindTextureID(textureId, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
}

void pl::Texture::setLinearFilter(bool value)
{
    if (textureId == 0)
    {
        return;
    }

    bindTextureID(textureId, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, value ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, value ? GL_LINEAR : GL_NEAREST);
}

int pl::Texture::getWidth() const
{
    return width;
}

int pl::Texture::getHeight() const
{
    return height;
}

void pl::Texture::overwriteData(int width, int height, const void* data, uint32_t colorFormat, uint32_t memoryFormat)
{
    use();

    glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, memoryFormat, data);

    this->width = width;
    this->height = height;
}

void pl::Texture::bindTextureID(GLuint textureId, int unit)
{
    if (activeTextures[unit] != textureId)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, textureId);

        activeTextures[unit] = textureId;
    }
}

void pl::Texture::use(int unit) const
{
    if (textureId == 0)
    {
        return;
    }

    bindTextureID(textureId, unit);
}

GLuint pl::Texture::getID()
{
    return textureId;
}