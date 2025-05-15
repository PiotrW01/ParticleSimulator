#pragma once
#include <array>
#include "shaders.h"
class Camera2D;


class Texture
{
public:
    unsigned int VAO = -1, VBO = -1;
    unsigned int shaderProgram = -1;
    float posX = 0.0f, posY = 0.0f;
    float width = 0.0f, height = 0.0f;
    ~Texture();
    Texture(float posX, float posY);
    void draw(const Camera2D &cam);
    void setSize(float width, float height);
    void updatePosition();
    void move(float x, float y);
    void setPosition(float x, float y);
    void load(const char *filename);

private:
    std::array<VertexPosTexture, 6> vertices;
    unsigned int texPtr = 0;
    void prepare();
    void calcVertices();
};