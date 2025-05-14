#include "app.h"

int main(int argc, char const *argv[])
{
    App app;
    app.vSyncEnabled(false);
    app.setWindowSize(800, 600);
    app.run();

    return 0;
}