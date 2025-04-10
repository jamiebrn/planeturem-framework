#pragma once

#include <extlib/gl.h>
#include <string>
#include <iostream>

#include "Graphics/Image.hpp"

namespace pl
{

class Texture
{
public:
    Texture() = default;
    ~Texture();
    Texture(Texture& texture) = delete;

    bool loadTexture(const std::string& texturePath, bool mipmap = false);
    bool loadTexture(uint8_t* pixels, int width, int height, bool mipmap = false);

    void setFromAllocated(GLuint textureId, int width, int height);

    void setTextureRepeat(bool repeat);
    void setLinearFilter(bool value);

    int getWidth() const;
    int getHeight() const;

    void use(int unit = 0) const;
    GLuint getID();

private:
    GLuint textureId = 0;

    int width = 0;
    int height = 0;

};

}