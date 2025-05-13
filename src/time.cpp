#include "time.h"

float Time::deltaTime = 0.0f;

int64_t Time::lastFrameTime = getCurrentTime();

void Time::update()
{
    int64_t curr = getCurrentTime();
    deltaTime = (curr - lastFrameTime) / 1000.0f;
    lastFrameTime = curr;
}

int64_t Time::getCurrentTime()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}