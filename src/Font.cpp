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

    for (auto& characterSet : renderedCharacterSets)
    {
        glDeleteTextures(1, &characterSet.second.texture);
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

    return true;
}

void pl::Font::draw(RenderTarget& renderTarget, const Shader& shader, const TextDrawData& drawData)
{
    if (!createCharacterSet(drawData.size))
    {
        printf("ERROR: Failed to create character set of size %d\n", drawData.size);
        return;
    }

    renderTarget.bind();
    shader.use();

    const CharacterSet& characterSet = renderedCharacterSets.at(drawData.size);

    Vector2f textPos = drawData.position;

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

        fontVertices.addVertex(Vertex(
            Vector2f(xPos, yPos),
            drawData.color,
            characterData.textureUV.getPosition()
        ));
        fontVertices.addVertex(Vertex(
            Vector2f(xPos + characterData.size.x, yPos),
            drawData.color,
            characterData.textureUV.getPosition() + Vector2f(characterData.textureUV.width, 0)
        ));
        fontVertices.addVertex(Vertex(
            Vector2f(xPos, yPos + characterData.size.y),
            drawData.color,
            characterData.textureUV.getPosition() + Vector2f(0, characterData.textureUV.height)
        ));
        fontVertices.addVertex(Vertex(
            Vector2f(xPos + characterData.size.x, yPos),
            drawData.color,
            characterData.textureUV.getPosition() + Vector2f(characterData.textureUV.width, 0)
        ));
        fontVertices.addVertex(Vertex(
            Vector2f(xPos + characterData.size.x, yPos + characterData.size.y),
            drawData.color,
            characterData.textureUV.getPosition() + characterData.textureUV.getSize()
        ));
        fontVertices.addVertex(Vertex(
            Vector2f(xPos, yPos + characterData.size.y),
            drawData.color,
            characterData.textureUV.getPosition() + Vector2f(0, characterData.textureUV.height)
        ));

        textPos.x += characterData.advance >> 6;
    }

    glBindTexture(GL_TEXTURE_2D, characterSet.texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, drawData.smoothing ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, drawData.smoothing ? GL_LINEAR : GL_NEAREST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fontVertices.draw(renderTarget);

    glBindTexture(GL_TEXTURE_2D, 0);
}

bool pl::Font::createCharacterSet(uint32_t size)
{
    if (renderedCharacterSets.contains(size))
    {
        return true;
    }
    
    FT_Set_Pixel_Sizes(fontFace, 0, size);

    int textureWidth = std::min(size * CHAR_COUNT, MAX_TEXTURE_WIDTH);
    int textureHeight = std::ceil(CHAR_COUNT / std::floor(textureWidth / size)) * size;

    std::vector<uint8_t> renderedGlyphs(textureWidth * textureHeight, 0);
    
    CharacterSet characterSet;

    int textureIdxX = 0;
    int textureIdxY = 0;

    for (uint32_t i = 0; i < CHAR_COUNT; i++)
    {
        if (FT_Load_Char(fontFace, i, FT_LOAD_RENDER))
        {
            return false;
        }
        
        Character character;
        character.size = Vector2<int>(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows);
        character.bearing = Vector2<int>(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top);
        character.advance = fontFace->glyph->advance.x;
        character.textureUV.x = textureIdxX * size / static_cast<float>(textureWidth);
        character.textureUV.y = textureIdxY * size / static_cast<float>(textureHeight);
        character.textureUV.width = fontFace->glyph->bitmap.width / static_cast<float>(textureWidth);
        character.textureUV.height = fontFace->glyph->bitmap.rows / static_cast<float>(textureHeight);

        for (int charY = 0; charY < character.size.y; charY++)
        {
            uint32_t rowIdx = (textureIdxY * size + charY) * textureWidth + textureIdxX * size;

            // Add another row if required
            if (rowIdx >= renderedGlyphs.size())
            {
                renderedGlyphs.resize(renderedGlyphs.size() + textureWidth * size);
            }

            uint8_t* glyphRow = &renderedGlyphs[rowIdx];

            memcpy(glyphRow, &fontFace->glyph->bitmap.buffer[character.size.x * charY], character.size.x);
        }

        characterSet.characterData[i] = character;

        textureIdxX++;
        if (textureIdxX * size > textureWidth - size)
        {
            textureIdxX = 0;
            textureIdxY++;
        }
    }

    glGenTextures(1, &characterSet.texture);
    glBindTexture(GL_TEXTURE_2D, characterSet.texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, renderedGlyphs.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    renderedCharacterSets[size] = characterSet;

    return true;
}