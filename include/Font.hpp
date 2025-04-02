#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <gl.h>
#include <SDL.h>

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "Vector.hpp"

namespace pl
{

class Font
{
public:
    Font(const std::string& fontPath);
    ~Font();
    Font(const Font& font) = delete;

private:
    bool createCharacterSet(uint32_t size);

    static FT_Library freetype;

    FT_Face fontFace;

    struct Character
    {
        // unsigned int textureID;  // ID handle of the glyph texture
        Vector2<int> size;       // Size of glyph
        Vector2<int> bearing;    // Offset from baseline to left/top of glyph
        uint32_t advance;    // Offset to advance to next glyph

        Vector2f textureUV;
    };

    struct CharacterSet
    {
        std::unordered_map<char, Character> characterData;
        GLuint texture;
    };

    std::unordered_map<uint32_t, CharacterSet> renderedCharacterSets;

};

}