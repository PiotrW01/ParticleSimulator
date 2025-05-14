#include "control.h"
#include "textureloader.h"
#include "texture.h"

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
    texture = std::make_unique<Texture>(x, y, width, height);
    texture->loadTexture(fileName);
}

void Control::render()
{
    if (texture)
    {
        texture->draw();
    }
}

void Control::update()
{
    if (texture)
    {
        texture->setPosition(x, y);
        texture->updatePosition();
    }
}