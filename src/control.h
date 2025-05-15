#pragma once
#include <memory>
#include "event.h"
class Texture;
class Camera2D;

class Control
{
public:
    Control(float x, float y, float width, float height);
    virtual ~Control() = default;
    float x, y, width, height;
    Event onClick;
    Event onDrag;
    Event onHold;
    Event onRelease;
    void render(Camera2D& cam);
    bool contains(float x, float y);
    void loadTexture(const char *fileName);
    void update();

private:
    std::unique_ptr<Texture> texture = nullptr;
};