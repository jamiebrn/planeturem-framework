#include "Font.hpp"

FT_Library pl::Font::freetype = nullptr;

pl::Font::Font()
{
    fontFace = nullptr;
}

pl::Font::Font(const std::string& fontPath)
{
    loadFromFile(fontPath);
}

pl::Font::~Font()
{
    if (!fontFace)
    {
        return;
    }

    FT_Done_Face(fontFace);
    FT_Stroker_Done(fontStroker);

    for (auto& characterSet : renderedCharacterSets)
    {
        glDeleteTextures(1, &characterSet.second.texture);
    }

    for (auto& outlineCharacterSets : renderedOutlineCharacterSets)
    {
        for (auto& outlineCharacterSet : outlineCharacterSets.second)
        {
            glDeleteTextures(1, &outlineCharacterSet.second.texture);
        }
    }
}

bool pl::Font::loadFromFile(const std::string& fontPath)
{
    if (!freetype)
    {
        if (FT_Init_FreeType(&freetype))
        {
            printf("ERROR: Failed to initialise FreeType\n");
            return false;
        }
    }
    
    if (FT_New_Face(freetype, fontPath.c_str(), 0, &fontFace))
    {
        printf(("ERROR: Failed to load font " + fontPath + "\n").c_str());
        return false;
    }
    
    if (FT_Stroker_New(freetype, &fontStroker))
    {
        printf(("ERROR: Failed to load font stroker " + fontPath + "\n").c_str());
        FT_Done_Face(fontFace);
        return false;
    }

    return true;
}

void pl::Font::draw(RenderTarget& renderTarget, Shader& shader, const TextDrawData& drawData)
{
    if (!createCharacterSet(drawData.size, 0))
    {
        printf("ERROR: Failed to create character set of size %d, outline 0\n", drawData.size);
        return;
    }
    if (drawData.outlineThickness > 0)
    {
        if (!createCharacterSet(drawData.size, drawData.outlineThickness))
        {
            printf("ERROR: Failed to create character set of size %d, outline %d\n", drawData.size, drawData.outlineThickness);
            return;
        }
    }

    renderTarget.bind();
    shader.use();

    const CharacterSet& characterSet = renderedCharacterSets.at(drawData.size);

    Vector2f textPos = drawData.position;

    VertexArray outlineFontVertices;
    VertexArray fontVertices;
    for (char character : drawData.text)
    {
        const Character& characterData = characterSet.characterData.at(character);

        if (character == '\n')
        {
            textPos.y += drawData.size;
            textPos.x = drawData.position.x;
            continue;
        }

        float xPos = textPos.x + characterData.bearing.x;
        float yPos = textPos.y + drawData.size - characterData.bearing.y;

        if (drawData.outlineThickness > 0)
        {
            const CharacterSet& outlineCharacterSet = renderedOutlineCharacterSets.at(drawData.size).at(drawData.outlineThickness);
            const Character& characterOutlineData = outlineCharacterSet.characterData.at(character);

            outlineFontVertices.addQuad(Rect<float>(xPos - (characterOutlineData.size.x - characterData.size.x) / 2,
                                                    yPos - (characterOutlineData.size.y - characterData.size.y) / 2,
                        characterOutlineData.size.x, characterOutlineData.size.y), drawData.outlineColor,
            Rect<float>(characterOutlineData.textureUV.x / outlineCharacterSet.textureWidth, characterOutlineData.textureUV.y / outlineCharacterSet.textureHeight,
                        characterOutlineData.textureUV.width / outlineCharacterSet.textureWidth, characterOutlineData.textureUV.height / outlineCharacterSet.textureHeight));
        }

        fontVertices.addQuad(Rect<float>(xPos, yPos, characterData.size.x, characterData.size.y), drawData.color,
            Rect<float>(characterData.textureUV.x / characterSet.textureWidth, characterData.textureUV.y / characterSet.textureHeight,
                        characterData.textureUV.width / characterSet.textureWidth, characterData.textureUV.height / characterSet.textureHeight));

        textPos.x += characterData.advance >> 16;
    }

    if (outlineFontVertices.size() > 0)
    {
        const CharacterSet& outlineCharacterSet = renderedOutlineCharacterSets.at(drawData.size).at(drawData.outlineThickness);
        glBindTexture(GL_TEXTURE_2D, outlineCharacterSet.texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, drawData.smoothing ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, drawData.smoothing ? GL_LINEAR : GL_NEAREST);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        outlineFontVertices.draw(renderTarget);
    }

    glBindTexture(GL_TEXTURE_2D, characterSet.texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, drawData.smoothing ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, drawData.smoothing ? GL_LINEAR : GL_NEAREST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fontVertices.draw(renderTarget);

    glBindTexture(GL_TEXTURE_2D, 0);
}

bool pl::Font::createCharacterSet(uint32_t size, uint32_t outline)
{
    if (outline <= 0)
    {
        if (renderedCharacterSets.contains(size))
        {
            return true;
        }
    }
    else
    {
        if (renderedOutlineCharacterSets[size].contains(outline))
        {
            return true;
        }
    }
    
    FT_Set_Pixel_Sizes(fontFace, 0, size);

    uint32_t charSize = size + outline * 2;

    int textureWidth = std::min(charSize * CHAR_COUNT, MAX_TEXTURE_WIDTH);
    int textureHeight = std::ceil(CHAR_COUNT / std::floor(textureWidth / charSize)) * charSize;

    std::vector<uint8_t> renderedGlyphs(textureWidth * textureHeight, 0);
    
    CharacterSet characterSet;

    int textureIdxX = 0;
    int textureIdxY = 0;

    int32_t loadFlags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;
    if (outline != 0)
    {
        loadFlags |= FT_LOAD_NO_BITMAP;
    }

    for (uint32_t i = 0; i < CHAR_COUNT; i++)
    {
        if (FT_Load_Char(fontFace, i, loadFlags))
        {
            return false;
        }

        FT_Glyph glyph;
        if (FT_Get_Glyph(fontFace->glyph, &glyph))
        {
            return false;
        }

        if (outline != 0 && glyph->format == FT_GLYPH_FORMAT_OUTLINE)
        {
            FT_Stroker_Set(fontStroker, outline << 6, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
            FT_Glyph_Stroke(&glyph, fontStroker, true);
        }

        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, true);
        FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

        Character character;
        character.size = Vector2<int>(bitmapGlyph->bitmap.width, bitmapGlyph->bitmap.rows);
        character.bearing = Vector2<int>(bitmapGlyph->left, bitmapGlyph->top);
        character.advance = bitmapGlyph->root.advance.x;
        character.textureUV.x = textureIdxX * charSize;
        character.textureUV.y = textureIdxY * charSize;
        character.textureUV.width = bitmapGlyph->bitmap.width;
        character.textureUV.height = bitmapGlyph->bitmap.rows;

        for (int charY = 0; charY < character.size.y; charY++)
        {
            uint32_t rowIdx = (textureIdxY * charSize + charY) * textureWidth + textureIdxX * charSize;

            // Add another row if required
            if (rowIdx >= renderedGlyphs.size())
            {
                renderedGlyphs.resize(renderedGlyphs.size() + textureWidth * charSize);
                textureHeight += charSize;
            }

            uint8_t* glyphRow = &renderedGlyphs[rowIdx];

            memcpy(glyphRow, &bitmapGlyph->bitmap.buffer[character.size.x * charY], character.size.x);
        }

        characterSet.characterData[i] = character;

        textureIdxX++;
        if (textureIdxX * charSize > textureWidth - charSize)
        {
            textureIdxX = 0;
            textureIdxY++;
        }
    }

    characterSet.textureHeight = textureHeight;
    characterSet.textureWidth = textureWidth;

    glGenTextures(1, &characterSet.texture);
    glBindTexture(GL_TEXTURE_2D, characterSet.texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, renderedGlyphs.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    if (outline == 0)
    {
        renderedCharacterSets[size] = characterSet;
    }
    else
    {
        renderedOutlineCharacterSets[size][outline] = characterSet;
    }

    return true;
}