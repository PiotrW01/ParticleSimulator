#pragma once
#include "glad.h"
#include <GLFW/glfw3.h>
#include "gui/gui.h"
#include "graphics/camera2d.h"

class App
{
public:
    App();
    ~App();
    void run();
    void vSyncEnabled(bool enabled);
    void setWindowSize(int width, int height);
    void setTargetFPS(int fps);
    void setTargetTicks(int ticks);

private:
    Camera2D cam;
    GLFWwindow *window = nullptr;
    GUI gui;
    int targetFPS = 0;
    int targetTicks = 20;
    double targetFrameTime = 0.0;
    double targetTickTime = 1.0 / targetTicks;
private:
    bool initializeWindow();
};