#include "Graphics/Font.hpp"

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

    if (fontFace)
    {
        FT_Done_Face(fontFace);
    }
    
    if (FT_New_Face(freetype, fontPath.c_str(), 0, &fontFace))
    {
        printf(("ERROR: Failed to load font " + fontPath + "\n").c_str());
        return false;
    }

    if (fontStroker)
    {
        FT_Stroker_Done(fontStroker);
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
    std::unordered_set<uint8_t> glyphChars;
    for (uint8_t character : drawData.text)
    {
        glyphChars.insert(character);
    }

    if (!createCharacterSetGlyphs(glyphChars, drawData.size, 0))
    {
        printf("ERROR: Failed to create character set of size %d, outline 0\n", drawData.size);
        return;
    }
    if (drawData.outlineThickness > 0)
    {
        if (!createCharacterSetGlyphs(glyphChars, drawData.size, drawData.outlineThickness))
        {
            printf("ERROR: Failed to create character set of size %d, outline %d\n", drawData.size, drawData.outlineThickness);
            return;
        }
    }

    renderTarget.bind();
    shader.bind();

    const CharacterSet& characterSet = renderedCharacterSets.at(drawData.size);

    // Null if not centring / clamping to area
    Rect<float> textBounds(0, 0, 0, 0);
    if (drawData.centeredX || drawData.centeredY || drawData.containOnScreenX || drawData.containOnScreenY)
    {
        textBounds = measureText(drawData);
    }

    Vector2f textStartPos = drawData.position;
    if (drawData.centeredX)
    {
        textBounds.x -= textBounds.width / 2;
        textStartPos.x = textBounds.x;
    }
    if (drawData.centeredY)
    {
        textBounds.y -= textBounds.height / 2;
        textStartPos.y = textBounds.y;
    }

    if (drawData.containOnScreenX)
    {
        textBounds.x = std::clamp(textBounds.x, drawData.containPaddingLeft, renderTarget.getWidth() - drawData.containPaddingRight - textBounds.width);
        textStartPos.x = textBounds.x;
    }
    if (drawData.containOnScreenY)
    {
        textBounds.y = std::clamp(textBounds.y, drawData.containPaddingTop, renderTarget.getHeight() - drawData.containPaddingBottom - textBounds.height);
        textStartPos.y = textBounds.y;
    }

    Vector2f textPos = textStartPos;

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
            Rect<float>(characterOutlineData.textureUV.x / TEXTURE_WIDTH, characterOutlineData.textureUV.y / outlineCharacterSet.textureHeight,
                        characterOutlineData.textureUV.width / TEXTURE_WIDTH, characterOutlineData.textureUV.height / outlineCharacterSet.textureHeight));
        }

        fontVertices.addQuad(Rect<float>(xPos, yPos, characterData.size.x, characterData.size.y), drawData.color,
            Rect<float>(characterData.textureUV.x / TEXTURE_WIDTH, characterData.textureUV.y / characterSet.textureHeight,
                        characterData.textureUV.width / TEXTURE_WIDTH, characterData.textureUV.height / characterSet.textureHeight));

        textPos.x += characterData.advance >> 16;
    }

    if (outlineFontVertices.size() > 0)
    {
        const CharacterSet& outlineCharacterSet = renderedOutlineCharacterSets.at(drawData.size).at(drawData.outlineThickness);
        glBindTexture(GL_TEXTURE_2D, outlineCharacterSet.texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, drawData.smoothing ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, drawData.smoothing ? GL_LINEAR : GL_NEAREST);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        outlineFontVertices.draw(renderTarget, nullptr);
    }

    glBindTexture(GL_TEXTURE_2D, characterSet.texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, drawData.smoothing ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, drawData.smoothing ? GL_LINEAR : GL_NEAREST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fontVertices.draw(renderTarget, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
}

pl::Rect<float> pl::Font::measureText(const TextDrawData& drawData)
{
    if (drawData.text.empty())
    {
        return Rect<float>(drawData.position.x, drawData.position.y, 0, 0);
    }

    const CharacterSet& characterSet = renderedCharacterSets.at(drawData.size);

    Vector2f textPos = drawData.position;

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

        textPos.x += characterData.advance >> 16;
    }

    Rect<float> bounds;
    bounds.x = drawData.position.x;
    bounds.y = drawData.position.y;
    bounds.width = textPos.x - drawData.position.x;
    bounds.height = textPos.y + drawData.size - drawData.position.y;

    if (drawData.outlineThickness > 0)
    {
        const Character& characterData = characterSet.characterData.at(drawData.text[0]);

        const CharacterSet& outlineCharacterSet = renderedOutlineCharacterSets.at(drawData.size).at(drawData.outlineThickness);
        const Character& characterOutlineData = outlineCharacterSet.characterData.at(drawData.text[0]);

        int xDiff = characterOutlineData.size.x - characterData.size.x;
        int yDiff = characterOutlineData.size.y - characterData.size.y;
        bounds.x -= xDiff / 2;
        bounds.y -= yDiff / 2;
        bounds.width += xDiff;
        bounds.height += yDiff;
    }

    return bounds;
}

bool pl::Font::createCharacterSetGlyphs(const std::unordered_set<uint8_t>& glyphChars, uint32_t size, uint32_t outline)
{
    CharacterSet* characterSet = nullptr;

    if (outline <= 0)
    {
        characterSet = &renderedCharacterSets[size];
    }
    else
    {
        characterSet = &renderedOutlineCharacterSets[size][outline];
    }
    
    FT_Set_Pixel_Sizes(fontFace, 0, size);

    uint32_t charSize = size + outline * 2;

    int32_t loadFlags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;
    if (outline != 0)
    {
        loadFlags |= FT_LOAD_NO_BITMAP;
    }

    bool rendered = false;
    
    for (uint8_t glyphChar : glyphChars)
    {
        if (characterSet->renderedGlyphs.contains(glyphChar))
        {
            continue;
        }

        if (FT_Load_Char(fontFace, glyphChar, loadFlags))
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
        character.textureUV.x = characterSet->textureRenderIdxX * charSize;
        character.textureUV.y = characterSet->textureRenderIdxY * charSize;
        character.textureUV.width = bitmapGlyph->bitmap.width;
        character.textureUV.height = bitmapGlyph->bitmap.rows;

        for (int charY = 0; charY < character.size.y; charY++)
        {
            uint32_t rowIdx = (characterSet->textureRenderIdxY * charSize + charY) * TEXTURE_WIDTH + characterSet->textureRenderIdxX * charSize;

            // Add another row if required
            if (rowIdx >= characterSet->renderedGlyphsBitmap.size())
            {
                characterSet->renderedGlyphsBitmap.resize(characterSet->renderedGlyphsBitmap.size() + TEXTURE_WIDTH * charSize);
                characterSet->textureHeight += charSize;
            }

            uint8_t* glyphRow = &characterSet->renderedGlyphsBitmap[rowIdx];

            memcpy(glyphRow, &bitmapGlyph->bitmap.buffer[character.size.x * charY], character.size.x);
        }

        characterSet->characterData[glyphChar] = character;

        rendered = true;
        characterSet->renderedGlyphs.insert(glyphChar);

        characterSet->textureRenderIdxX++;
        if (characterSet->textureRenderIdxX * charSize > TEXTURE_WIDTH - charSize)
        {
            characterSet->textureRenderIdxX = 0;
            characterSet->textureRenderIdxY++;
        }
    }

    if (rendered)
    {
        glGenTextures(1, &characterSet->texture);
        glBindTexture(GL_TEXTURE_2D, characterSet->texture);
    
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TEXTURE_WIDTH, characterSet->textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, characterSet->renderedGlyphsBitmap.data());
    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }

    return true;
}