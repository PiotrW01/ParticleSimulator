#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm.hpp>
#include <unordered_map>
#include <vector>

struct Glyph {
    glm::ivec2 size;     // width, height in pixels
    glm::ivec2 bearing;  // offset from baseline to left/top
    unsigned int advance; // advance.x
    glm::vec2 uv0;       // top-left in atlas
    glm::vec2 uv1;       // bottom-right in atlas
};

class FontAtlas {
public:
    bool load(const char* fontPath, int pixelHeight = 48);
    void destroy();

    unsigned int getTextureID() const { return textureID; }
    const Glyph& getGlyph(char c) const;

private:
    std::unordered_map<char, Glyph> glyphs;
    unsigned int textureID = 0;
    int atlasWidth = 0;
    int atlasHeight = 0;
};