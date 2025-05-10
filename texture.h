#pragma once
#include <GLFW/glfw3.h>
#include <array>

class Texture
{
public:
    unsigned int VAO, VBO;
    unsigned int shaderProgram;
    float posX, posY = 0.0f;
    int width, height = 0;
    Texture();
    ~Texture();
    Texture(int posX, int posY, int width, int height);
    void draw();
    void updatePosition();
    void move(float x, float y);
    void setPosition(float x, float y);
    bool contains(float x, float y);
    void loadTexture(const char *filename);

private:
    std::array<float, 30> vertices;
    unsigned int texPtr = 0;
    void prepare();
    void calcVertices();
};