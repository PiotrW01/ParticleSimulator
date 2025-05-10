#pragma once
#include <chrono>

class FPSCounter
{
    public:
        static int fps;
        static void update();
    private:
        static int64_t lastTime;
};