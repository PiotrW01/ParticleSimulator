#pragma once
#include <chrono>

class Time
{
public:
    double deltaTime = 0.0;
    double currentTime = 0;
public:
    void update();
    double getCurrentTime();
private:
    double lastFrameTime = 0;
};