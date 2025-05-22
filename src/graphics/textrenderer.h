#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include "vec2.hpp"
#include "vec3.hpp"
#include <map>
#include <string>
class Camera2D;

struct Character {
    unsigned int textureID;  // ID handle of the glyph texture
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
};

class TextRenderer
{
public:
	void init();
    void RenderText(Camera2D& cam, std::string text, float x, float y, float scale, glm::vec3 color);
private:
    unsigned int VAO, VBO, shaderProgram;
    std::map<char, Character> Characters;
};