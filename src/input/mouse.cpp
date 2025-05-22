#include "mouse.h"
#include "GLFW/glfw3.h"
#include "inputmanager.h"
#include "graphics/camera2d.h"
#include "utils/globals.h"
#include "gui/control.h"

std::vector<std::shared_ptr<Control>> Mouse::controls;
glm::vec2 Mouse::mouseDelta = glm::vec2(0.0f, 0.0f);
glm::vec2 Mouse::cameraPos = glm::vec2(0.0f, 0.0f);
double Mouse::lastX = 0.0, Mouse::lastY = 0.0;
float Mouse::cameraZoom = 1.0f;

void Mouse::init(GLFWwindow *window)
{
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    lastX = xPos;
    lastY = WINDOW_HEIGHT - yPos;
}

glm::vec2 Mouse::getMousePos()
{
    return glm::vec2(lastX, lastY);
}

glm::vec2 Mouse::getWorldMousePos()
{
    glm::vec2 screenOffset = glm::vec2(lastX, lastY) - glm::vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    return (cameraPos + screenOffset) / cameraZoom;
}

void Mouse::update(GLFWwindow *window, Camera2D &cam)
{
    cameraPos = cam.position;
    cameraZoom = cam.zoom;
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    yPos = WINDOW_HEIGHT - yPos;
    mouseDelta = glm::vec2(xPos - lastX, yPos - lastY) / cameraZoom;
    lastX = xPos;
    lastY = yPos;

    if (InputManager::isKeyJustReleased(Btn::LEFT))
    {
        controls.clear();
    }
    for (auto& control : controls)
    {
        control->x += mouseDelta.x;
        control->y += mouseDelta.y;
    }
}

void Mouse::attachControl(std::shared_ptr<Control> c)
{
    controls.push_back(std::move(c));
}
