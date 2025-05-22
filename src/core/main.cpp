#ifdef _WIN32
#include <Windows.h>
int main(int argc, char const* argv[]);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, const_cast<const char**>(__argv));
}
#endif

#include "app.h"
int main(int argc, char const *argv[])
{
    App app;
    app.vSyncEnabled(false);
    app.setWindowSize(800, 600);
    app.setTargetTicks(20);
    app.run();

    return 0;
}