#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_STROKER_H
#include <extlib/gl.h>
#include <SDL.h>

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

#include "Vector.hpp"
#include "Rect.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
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

    void draw(RenderTarget& renderTarget, Shader& shader, const TextDrawData& drawData);

    pl::Rect<int> measureText(const TextDrawData& drawData);

private:
    bool createRequiredGlyphs(const TextDrawData& drawData);
    bool createCharacterSetGlyphs(const std::unordered_set<uint8_t>& glyphChars, uint32_t size, uint32_t outline);

    static inline const int CHAR_COUNT = 128;
    static inline const uint32_t TEXTURE_WIDTH = 2048;

    static FT_Library freetype;

    FT_Face fontFace;
    FT_Stroker fontStroker;

    struct Character
    {
        Vector2<int> size;
        Vector2<int> bearing;
        uint32_t advance;

        Rect<float> textureUV;
    };

    struct CharacterSet
    {
        std::unordered_map<uint8_t, Character> characterData;

        std::vector<uint8_t> renderedGlyphsBitmap;
        std::unordered_set<uint8_t> renderedGlyphs;
        uint32_t textureRenderIdxX = 0;
        uint32_t textureRenderIdxY = 0;

        Texture texture;
        
        // GLuint texture;
        // uint32_t textureHeight = 0;
    };

    std::unordered_map<uint32_t, CharacterSet> renderedCharacterSets;

    std::unordered_map<uint32_t, std::unordered_map<uint32_t, CharacterSet>> renderedOutlineCharacterSets;

};

}