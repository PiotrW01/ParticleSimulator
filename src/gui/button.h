#pragma once
#include "input/event.h"
#include "control.h"

class Button : public Control
{
public:
    using Control::Control;
    bool isPressed = false;
};