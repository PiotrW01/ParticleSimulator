#include "fpscounter.h"
#include "logger.h"

void FPSCounter::update(double deltaTime)
{
    acc += deltaTime;
    if (acc >= refreshTime)
    {
        LOG_INFO << "FPS: " << fps / refreshTime;
        acc = 0.0;
        fps = 0;
    }
}

void FPSCounter::addFrame()
{
    fps++;
}