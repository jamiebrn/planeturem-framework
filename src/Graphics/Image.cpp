#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/Image.hpp"

Image::Image()
{
    width = 0;
    height = 0;
    channels = 0;
    pixels = nullptr;
}

Image::~Image()
{
    if (pixels)
    {
        stbi_image_free(pixels);
    }
}

bool Image::loadFromFile(const std::string& filePath)
{
    pixels = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    return (pixels != nullptr);
}

int Image::getWidth() const
{
    return width;
}

int Image::getHeight() const
{
    return height;
}

int Image::getByteSize() const
{
    return width * height * 4;
}

uint8_t* Image::getPixelPtr() const
{
    return pixels;
}