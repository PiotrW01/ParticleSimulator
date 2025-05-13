#pragma once
#include <vector>
#include <memory>
class Texture;
class Control;

class GUI
{
public:
    GUI();
    ~GUI();
    void test();
    void render();
    void update();
    std::vector<std::unique_ptr<Control>> controls;

private:
};