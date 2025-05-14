#include "gui.h"
#include "texture.h"
#include "mouse.h"
#include "button.h"
#include "inputmanager.h"
#include "logger.h"

GUI::GUI() {
};

GUI::~GUI() = default;

void GUI::test()
{
    return;
    std::unique_ptr<Button> btn = std::make_unique<Button>(10, 10, 100, 100);
    Button *btnPtr = btn.get();
    btn->onClick.subscribe([btnPtr]()
                           {
        Logger::info() << "button clicked!";
        btnPtr->isPressed = true; });
    btn->onRelease.subscribe([btnPtr]()
                             {
        Logger::info() << "button released!";
        btnPtr->isPressed = false; });
    btn->onHold.subscribe([btnPtr]
                          {
        btnPtr->x += Mouse::mouseDelta.x;
        btnPtr->y += Mouse::mouseDelta.y; });
    btn->loadTexture("123.png");
    controls.push_back(std::move(btn));
}

void GUI::update()
{
    glm::vec2 pos = Mouse::getMousePos();
    for (auto &control : controls)
    {
        if (control->contains(pos.x, pos.y))
        {
            if (InputManager::isKeyDown(Btn::LEFT))
            {
                control->onClick.trigger();
            }
            else if (InputManager::isKeyUp(Btn::LEFT))
            {
                control->onRelease.trigger();
            }
            else if (InputManager::isKeyHeld(Btn::LEFT))
            {
                control->onHold.trigger();
            }
        }
        control->update();
    }
}

void GUI::render()
{
    for (auto &control : controls)
    {
        control->render();
    }
}