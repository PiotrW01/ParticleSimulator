#pragma once
#include <vector>
#include "vec2.hpp"
#include <memory>
class Texture;
struct GLFWwindow;
class Camera2D;

class Mouse
{
public:
    static glm::vec2 mouseDelta;
    static glm::vec2 getMousePos();
    static glm::vec2 getWorldMousePos();
    static void init(GLFWwindow *window);
    static void update(GLFWwindow *window, Camera2D &cam);
    static void attachSprite(Texture &sprite);
    static std::vector<Texture *> attachedSprites;

private:
    static float lastX, lastY;
    static glm::vec2 cameraPos;
    static float cameraZoom;
};
