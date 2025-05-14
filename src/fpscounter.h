#pragma once
#include <chrono>

class FPSCounter
{
public:
    int fps = 0;
public:
    void update(double deltaTime);
    void addFrame();

private:
    double lastTime = 0;
    double acc = 0.0;
};