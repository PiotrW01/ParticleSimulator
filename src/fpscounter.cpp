#include "fpscounter.h"
#include "logger.h"

void FPSCounter::update(double deltaTime)
{
    acc += deltaTime;
    if (acc >= refreshTime)
    {
		fps = tempFPS / refreshTime;
        LOG_INFO << "FPS: " << fps / refreshTime;
        acc = 0.0;
        tempFPS = 0;
    }
}

void FPSCounter::addFrame()
{
    tempFPS++;
}