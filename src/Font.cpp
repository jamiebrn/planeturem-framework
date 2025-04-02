#include "Font.hpp"

FT_Library pl::Font::freetype = nullptr;

pl::Font::Font(const std::string& fontPath)
{
    if (!freetype)
    {
        if (FT_Init_FreeType(&freetype))
        {
            printf("ERROR: Failed to initialise FreeType\n");
            return;
        }
    }

    if (FT_New_Face(freetype, fontPath.c_str(), 0, &fontFace))
    {
        printf(("ERROR: Failed to load font " + fontPath + "\n").c_str());
        return;
    }
}

pl::Font::~Font()
{
    FT_Done_Face(fontFace);
}

bool pl::Font::createCharacterSet(uint32_t size)
{
    if (renderedCharacterSets.contains(size))
    {
        return true;
    }

    bool success = true;
    
    FT_Set_Pixel_Sizes(fontFace, 0, size);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, size * 128, size, 1, 0xFF000000, 0xFF0000, 0xFF00, 0xFF);

    CharacterSet characterSet;

    for (uint8_t i = 0; i < 128; i++)
    {
        if (FT_Load_Char(fontFace, i, FT_LOAD_RENDER))
        {
            success = false;
            break;
        }

        SDL_Surface* glyphSurface = SDL_CreateRGBSurface(0, fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows, 1, 0xFF000000, 0xFF0000, 0xFF00, 0xFF);

        
    }

    return success;
}