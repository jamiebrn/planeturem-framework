#pragma once

#include <gl.h>
#include <SDL.h>
#include <string>
#include <iostream>

#include "Image.hpp"

class Texture
{
public:
    Texture() = default;
    ~Texture();
    Texture(Texture& texture) = delete;

    bool loadTexture(const std::string& texturePath, bool mipmap = false);

    void setTextureRepeat(bool repeat);
    void setLinearFilter(bool value);

    void use() const;
    GLuint getID();

private:
    GLuint textureId = 0;

};