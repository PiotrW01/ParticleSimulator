#include "time.h"

void Time::update()
{
    currentTime = getCurrentTime();
    deltaTime = (currentTime - lastFrameTime);
    lastFrameTime = currentTime;
}

double Time::getCurrentTime()
{
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}