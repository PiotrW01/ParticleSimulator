#include "fpscounter.h"
#include "time.h"
#include "logger.h"

void FPSCounter::update(double deltaTime)
{
    acc += deltaTime;
    if (acc >= 1.0)
    {
        LOG_INFO << "FPS: " << fps;
        acc = 0.0;
        fps = 0;
    }
}

void FPSCounter::addFrame()
{
    fps++;
}