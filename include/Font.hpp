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
#include "Rect.hpp"
#include "RenderTarget.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "TextDrawData.hpp"

namespace pl
{

class Font
{
public:
    Font();
    Font(const std::string& fontPath);
    ~Font();
    Font(const Font& font) = delete;

    bool loadFromFile(const std::string& fontPath);

    void draw(RenderTarget& renderTarget, const Shader& shader, const TextDrawData& drawData);

private:
    bool createCharacterSet(uint32_t size);

    static inline const int CHAR_COUNT = 128;
    static inline const uint32_t MAX_TEXTURE_WIDTH = 2048;

    static FT_Library freetype;

    FT_Face fontFace;

    struct Character
    {
        // unsigned int textureID;  // ID handle of the glyph texture
        Vector2<int> size;       // Size of glyph
        Vector2<int> bearing;    // Offset from baseline to left/top of glyph
        uint32_t advance;    // Offset to advance to next glyph

        Rect<float> textureUV;
    };

    struct CharacterSet
    {
        std::unordered_map<char, Character> characterData;
        GLuint texture;
    };

    std::unordered_map<uint32_t, CharacterSet> renderedCharacterSets;

};

}