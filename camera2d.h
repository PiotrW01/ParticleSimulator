#pragma once
#include "glm/glm.hpp"

class Camera2D
{
public:
    glm::vec2 position = {0.0f, 0.0f};
    float zoom = 1.0f;
    float rotation = 0.0f;

    float viewportWidth, viewportHeight;
    Camera2D(float width, float height);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getCombinedMatrix() const;
};