#pragma once
#include "vec2.hpp"
#include "vec3.hpp"
#include <map>
#include <string>
#include "fontatlas.h"
class Camera2D;

class TextRenderer
{
public:
    void init();
    void RenderText(const Camera2D& cam, const std::string& text, float x, float y, float scale, glm::vec3& color, bool fixed = true);
public:
    const int fontSize = 16;
private:
    unsigned int bufferCapacity = 128;
    unsigned int VAO, VBO, shaderProgram;
    FontAtlas atlas;
};