#pragma once

#include <gl.h>
#include <string>
#include <iostream>

#include "Image.hpp"

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

    void setTextureRepeat(bool repeat);
    void setLinearFilter(bool value);

    int getWidth() const;
    int getHeight() const;

    void use(int unit = 0) const;
    GLuint getID();

private:
    GLuint textureId = 0;

    int width;
    int height;

};

}