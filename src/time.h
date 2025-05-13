#pragma once
#include <chrono>

class Time
{
    public:
        static float deltaTime;
        static void update();
        static int64_t getCurrentTime();
    private:
        static int64_t lastFrameTime;
};