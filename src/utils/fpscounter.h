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
	int tempFPS = 0;
    float refreshTime = 1.0f;
    double lastTime = 0;
    double acc = 0.0;
};