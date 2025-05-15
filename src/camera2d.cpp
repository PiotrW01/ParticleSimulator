#include "camera2d.h"
#include "gtc/matrix_transform.hpp"

Camera2D::Camera2D(float width, float height)
{
    viewportWidth = width;
    viewportHeight = height;
}

glm::mat4 Camera2D::getViewMatrix() const
{
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(-position, 0.0f));
    if (rotation != 0.0f)
        view = glm::rotate(view, -rotation, glm::vec3(0, 0, 1));
    view = glm::scale(view, glm::vec3(zoom, zoom, 1.0f));
    return view;
}

glm::mat4 Camera2D::getProjectionMatrix() const
{
    float halfW = viewportWidth / 2.0f;
    float halfH = viewportHeight / 2.0f;
    return glm::ortho(-halfW, halfW, -halfH, halfH);
}

glm::mat4 Camera2D::getCombinedMatrix() const
{
    return getProjectionMatrix() * getViewMatrix();
}