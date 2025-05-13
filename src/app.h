#pragma once
#include "glad.h"
#include <GLFW/glfw3.h>
class GUI;
class Camera2D;

class App
{
public:
    static Camera2D cam;
    static GLFWwindow *window;
    static GUI gui;
    void run();
    App();
    App(int width, int height);
    ~App();

private:
    bool initializeWindow();
};