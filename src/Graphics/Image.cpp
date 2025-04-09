#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/Image.hpp"

pl::Image::Image()
{
    width = 0;
    height = 0;
    channels = 0;
    pixels = nullptr;
}

pl::Image::~Image()
{
    if (pixels)
    {
        stbi_image_free(pixels);
    }
}

bool pl::Image::loadFromFile(const std::string& filePath)
{
    pixels = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    return (pixels != nullptr);
}

int pl::Image::getWidth() const
{
    return width;
}

int pl::Image::getHeight() const
{
    return height;
}

int pl::Image::getByteSize() const
{
    return width * height * 4;
}

uint8_t* pl::Image::getPixelPtr() const
{
    return pixels;
}

pl::Color pl::Image::getPixel(int x, int y) const
{
    uint8_t* pixelPtr = &pixels[(y * width + x) * 4];
    pl::Color color(*pixelPtr, *(pixelPtr + 1), *(pixelPtr + 2), *(pixelPtr + 3));
    return color;
}