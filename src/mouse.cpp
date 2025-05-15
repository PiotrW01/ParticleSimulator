#include "mouse.h"
#include "texture.h"
#include "GLFW/glfw3.h"
#include "inputmanager.h"
#include "camera2d.h"
#include "globals.h"

std::vector<Texture *> Mouse::attachedSprites;
glm::vec2 Mouse::mouseDelta = glm::vec2(0.0f, 0.0f);
glm::vec2 Mouse::cameraPos = glm::vec2(0.0f, 0.0f);
float Mouse::lastX = 0.0f, Mouse::lastY = 0.0f;
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

    if (InputManager::isKeyUp(Btn::LEFT))
    {
        attachedSprites.clear();
    }
    for (auto &sprite : attachedSprites)
    {
        sprite->move(mouseDelta.x, mouseDelta.y);
    }
}

void Mouse::attachSprite(Texture &sprite)
{
    for (auto *s : attachedSprites)
    {
        if (s == &sprite)
            return;
    }

    attachedSprites.push_back(&sprite);
}
