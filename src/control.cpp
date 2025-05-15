#include "control.h"
#include "textureloader.h"

Control::Control(float x, float y, float width, float height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

bool Control::contains(float x, float y)
{
    return (x >= this->x && x <= this->x + this->width) &&
           (y >= this->y && y <= this->y + this->height);
}

void Control::loadTexture(const char *fileName)
{
    texture = std::make_unique<Texture>(x, y);
    texture->load(fileName);
}

void Control::render(Camera2D& cam)
{
    if (texture)
    {
        texture->draw(cam);
    }
}

void Control::update()
{
    if (texture)
    {
        texture->setPosition(x, y);
    }
}