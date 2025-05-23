#include <iostream>
#include "fontatlas.h"
#include "glad.h"
#include "utils/logger.h"

bool FontAtlas::load(const char* fontPath, int pixelHeight) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) 
    {
        LOG_ERROR << "ERROR::FREETYPE: Could not init FreeType";
        return false;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face)) 
    {
        LOG_ERROR << "ERROR::FREETYPE: Failed to load font";
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, pixelHeight);

    // Estimate atlas size
    const int maxAtlasWidth = 256;
    const int padding = 1;
    const int maxGlyphs = 128;
    int rowWidth = 0, rowHeight = 0;
    atlasWidth = 0;
    atlasHeight = 0;

    // First pass to calculate atlas size
    for (int c = 32; c < maxGlyphs; c++) 
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;

        // if 
        if (rowWidth + face->glyph->bitmap.width + padding >= maxAtlasWidth) {
            atlasWidth = std::max(maxAtlasWidth, rowWidth);
            atlasHeight += rowHeight + padding;
            rowWidth = 0;
            rowHeight = 0;
        }

        rowWidth += face->glyph->bitmap.width + padding;
        rowHeight = std::max(rowHeight, (int)face->glyph->bitmap.rows);
    }
    atlasWidth = std::max(maxAtlasWidth, rowWidth);
    atlasHeight += rowHeight + padding;

    std::vector<unsigned char> atlasData(atlasWidth * atlasHeight, 0);

    // Second pass: pack glyphs
    int x = 0, y = 0;
    rowHeight = 0;

    for (int c = 32; c < maxGlyphs; c++) 
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;

        FT_Bitmap& bmp = face->glyph->bitmap;

        if (x + bmp.width + padding >= atlasWidth) 
        {
            x = 0;
            y += rowHeight + padding;
            rowHeight = 0;
        }
        if (x + bmp.width > atlasWidth || y + bmp.rows > atlasHeight) {
            LOG_ERROR << "Glyph '" << (char)c << "' doesn't fit in atlas ("
                << bmp.width << "x" << bmp.rows << " at " << x << "," << y
                << " vs atlas " << atlasWidth << "x" << atlasHeight << ")";
            continue;
        }

        // Copy bitmap into atlas
        for (int i = 0; i < bmp.rows; ++i) 
        {
            for (int j = 0; j < bmp.width; ++j) 
            {
                atlasData[(y + i) * atlasWidth + (x + j)] =
                    bmp.buffer[i * bmp.pitch + j];
            }
        }

        Glyph glyph;
        glyph.size = glm::ivec2(bmp.width, bmp.rows);
        glyph.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        glyph.advance = face->glyph->advance.x;
        glyph.uv0 = glm::vec2((float)x / atlasWidth, (float)y / atlasHeight);
        glyph.uv1 = glm::vec2((float)(x + bmp.width) / atlasWidth,
            (float)(y + bmp.rows) / atlasHeight);

        glyphs[c] = glyph;

        x += bmp.width + padding;
        rowHeight = std::max(rowHeight, (int)bmp.rows);
    }

    // Generate OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, atlasData.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return true;
}

void FontAtlas::destroy() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
    glyphs.clear();
}

const Glyph& FontAtlas::getGlyph(char c) const {
    static Glyph fallback{};
    auto it = glyphs.find(c);
    return it != glyphs.end() ? it->second : fallback;
}