#pragma once

#include <cstdint>
#include <extlib/stb_image.h>

#include <string>

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

private:
    int width;
    int height;
    int channels;
    uint8_t* pixels;

};