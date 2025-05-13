#include "fpscounter.h"
#include "time.h"
#include "logger.h"

int FPSCounter::fps = 0;
int64_t FPSCounter::lastTime = Time::getCurrentTime();

void FPSCounter::update()
{
    if ((Time::getCurrentTime() - lastTime) >= 1000)
    {
        lastTime = Time::getCurrentTime();
        Logger::info(fps);
        fps = 0;
    }
    fps++;
}