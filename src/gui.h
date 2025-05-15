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
    std::vector<std::shared_ptr<Control>> controls;
    bool isMouseOverUI();
private:
    bool mouseOverUI = false;
};