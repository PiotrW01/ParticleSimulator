#pragma once
#include <vector>
#include "vec2.hpp"
#include <memory>
struct GLFWwindow;
class Camera2D;
class Control;

class Mouse
{
public:
    static glm::vec2 mouseDelta;
    static glm::vec2 getMousePos();
    static glm::vec2 getWorldMousePos();
    static void init(GLFWwindow *window);
    static void update(GLFWwindow *window, Camera2D &cam);
    static void attachControl(std::shared_ptr<Control> c);

private:
    static std::vector<std::shared_ptr<Control>> controls;
    static float lastX, lastY;
    static glm::vec2 cameraPos;
    static float cameraZoom;
};
