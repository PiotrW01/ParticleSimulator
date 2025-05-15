#include "fpscounter.h"
#include "logger.h"

void FPSCounter::update(double deltaTime)
{
    acc += deltaTime;
    if (acc >= 5.0)
    {
        LOG_INFO << "FPS: " << fps / 5.0;
        acc = 0.0;
        fps = 0;
    }
}

void FPSCounter::addFrame()
{
    fps++;
}