#include "gui.h"
#include "mouse.h"
#include "button.h"
#include "inputmanager.h"
#include "logger.h"

GUI::GUI() {
};

GUI::~GUI() = default;

void GUI::init()
{
    std::shared_ptr<Button> btn = std::make_shared<Button>(10, 10, 100, 100);
    btn->onClick.subscribe([btn]()
        {
            btn.get()->isPressed = true;
            Mouse::attachControl(btn);
        });

    btn->onRelease.subscribe([btn]()
        {
            btn.get()->isPressed = false;
        });

    btn->onHold.subscribe([btn]
        {
        });
    btn->loadTexture("assets/123.png");
    controls.push_back(std::move(btn));
}

bool GUI::isMouseOverUI()
{
    return mouseOverUI;
}

void GUI::update()
{
    mouseOverUI = false;
    glm::vec2 pos = Mouse::getWorldMousePos();
    for (auto &control : controls)
    {
        if (control->contains(pos.x, pos.y))
        {
            mouseOverUI = true;
            control->onHover.trigger();

            if (InputManager::isKeyJustPressed(Btn::LEFT))
            {
                control->onClick.trigger();
            }
            else if (InputManager::isKeyJustReleased(Btn::LEFT))
            {
                control->onReleaseOver.trigger();
            }
            else if (InputManager::isKeyDown(Btn::LEFT))
            {
                control->onHoldOver.trigger();
            }
        }
        control->update();
    }
}

void GUI::render(Camera2D& cam)
{
    for (auto &control : controls)
    {
        control->render(cam);
    }
}