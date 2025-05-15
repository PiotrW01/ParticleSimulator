#pragma once
#include <vector>
#include <memory>
class Texture;
class Control;
class Camera2D;

class GUI
{
public:
    GUI();
    ~GUI();
    void init();
    void render(Camera2D& cam);
    void update();
    std::vector<std::unique_ptr<Control>> controls;
    bool isTriggered();
private:
    bool triggered = false;
};