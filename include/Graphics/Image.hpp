#pragma once

#include <cstdint>
#include <extlib/stb_image.h>

#include <string>

#include <Graphics/Color.hpp>

namespace pl
{

class Image
{
public:
    Image();
    ~Image();
    bool loadFromFile(const std::string& filePath);

    int getWidth() const;
    int getHeight() const;
    int getByteSize() const;
    uint8_t* getPixelPtr() const;

    pl::Color getPixel(int x, int y) const;

private:
    int width;
    int height;
    int channels;
    uint8_t* pixels;

};

}