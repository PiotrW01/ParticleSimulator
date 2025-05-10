#include "app.h"
#include "const.h"
#include "globals.h"
#include <chrono>
#include "textureloader.h"
#include "fpscounter.h"
#include "inputmanager.h"
#include "gui.h"
#include "mouse.h"
#include "time.h"
#include "logger.h"
#include "mouse.h"
#include "particlesystem.h"
#include "camera2d.h"

GLFWwindow *App::window = nullptr;
GUI App::gui;
Camera2D App::cam(WINDOW_WIDTH, WINDOW_HEIGHT);

App::App()
{
}

App::App(int width, int height)
{
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
}

App::~App()
{
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
}

bool App::initializeWindow()
{
    if (!glfwInit())
    {
        Logger::error("glfw init error");
        return false;
    }

    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "window", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                   {
        WINDOW_WIDTH = width;
        WINDOW_HEIGHT = height;
                                    
        cam.viewportHeight = height;
        cam.viewportWidth = width;

        glViewport(0, 0, width, height); });
    // glfwSetWindowAspectRatio(window, 16, 9);
    glfwSetWindowSizeLimits(window, 320, 180, 7680, 4320);
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::error("glad init error");
        glfwTerminate();
        return false;
    }

    int x, y, n;
    unsigned char *data = TextureLoader::loadTexture("thumbnail.png", &x, &y, &n);
    GLFWimage img;
    img.width = x;
    img.height = y;
    img.pixels = data;

    glfwSetWindowIcon(window, 1, &img);
    TextureLoader::free(data);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    return true;
}

void App::run()
{
    if (!initializeWindow())
    {
        return;
    }

    cam.viewportWidth = WINDOW_WIDTH;
    cam.viewportHeight = WINDOW_HEIGHT;
    Mouse::init(window);
    // gui.test();

    ParticleSystem::init();

    glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset)
                          {
    if(yoffset > 0)
    {
        // zoom in
        App::cam.zoom += 0.1f;
    }
    else if(yoffset < 0)
    {
        if(App::cam.zoom > 0.1f)
        {
            App::cam.zoom -= 0.1f;

        }   
    } });

    bool step = false;

    while (!glfwWindowShouldClose(window))
    {
        Time::update();
        FPSCounter::update();
        glfwPollEvents();
        InputManager::update(window);
        Mouse::update(window, cam);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set background color
        glClear(GL_COLOR_BUFFER_BIT);         // Clear the screen
        std::stringstream ss;
        ss << "x: " << Mouse::getWorldMousePos().x << " y: " << Mouse::getWorldMousePos().y;
        Logger::info(ss.str());
        if (InputManager::isKeyHeld(Btn::LEFT))
        {
            ParticleSystem::spawnParticles(Mouse::getWorldMousePos(), ParticleType::Sand, 5);
        }
        if (InputManager::isKeyHeld(Key::ONE))
        {
            ParticleSystem::spawnParticles(Mouse::getWorldMousePos(), ParticleType::Water, 5);
        }

        if (InputManager::isKeyHeld(Btn::RIGHT))
        {
            cam.position -= Mouse::mouseDelta;
        }

        if (InputManager::isKeyDown(Key::Z))
            step = !step;

        if (step)
        {
            if (InputManager::isKeyDown(Key::TWO))
            {
                ParticleSystem::update();
            }
        }
        else
        {

            ParticleSystem::update();
        }
        ParticleSystem::render(cam);

        // gui.update();
        // gui.render();
        glfwSwapBuffers(window);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
}