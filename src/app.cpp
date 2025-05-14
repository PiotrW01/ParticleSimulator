#include "app.h"
#include "const.h"
#include "globals.h"
#include <chrono>
#include "textureloader.h"
#include "fpscounter.h"
#include "inputmanager.h"
#include "mouse.h"
#include "time.h"
#include "logger.h"
#include "particlesystem.h"


App::App()
{
    if (initializeWindow())
    {
        Logger::info() << "App initialized";
    }
    else
    {
        Logger::error() << "Error during app initialization";
    }
}

App::~App()
{
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
    Logger::info() << "App destroyed";
}

bool App::initializeWindow()
{
    if (!glfwInit())
    {
        Logger::error() << "glfw init error";
        return false;
    }

    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "window", NULL, NULL);

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
    {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));

        WINDOW_WIDTH = width;
        WINDOW_HEIGHT = height;
                                    
        app->cam.viewportWidth = width;
        app->cam.viewportHeight = height;
        glViewport(0, 0, width, height); 
    });
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
            if (yoffset > 0)
            {
                app->cam.zoom += 0.1f;
            }
            else if (yoffset < 0)
            {
                if (app->cam.zoom > 0.1f)
                {
                    app->cam.zoom -= 0.1f;
                }
            }
        });
    glfwSetWindowSizeLimits(window, 320, 180, 7680, 4320);
    glfwMakeContextCurrent(window);
    // glfwSetWindowAspectRatio(window, 16, 9);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::error() << "glad init error";
        glfwTerminate();
        return false;
    }

    int x, y, n;
    unsigned char *data = TextureLoader::loadTexture("assets/thumbnail.png", &x, &y, &n);
    GLFWimage img;
    img.width = x;
    img.height = y;
    img.pixels = data;

    if (data)
    {
        glfwSetWindowIcon(window, 1, &img);
    }
    TextureLoader::free(data);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    return true;
}

void App::run()
{
    cam.viewportWidth = WINDOW_WIDTH;
    cam.viewportHeight = WINDOW_HEIGHT;
    Mouse::init(window);
    // gui.test();

    ParticleSystem::init();

    bool step = false;

    double lastTickTime = glfwGetTime();
    double lastFrameTime = glfwGetTime();
    double accumulator = 0.0;

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        double deltaTime = now - lastTickTime;
        lastTickTime = now;
        accumulator += deltaTime;

        glfwPollEvents();
        InputManager::update(window);
        Mouse::update(window, cam);
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
        {
            step = !step;
        }
        if (step)
        {
            if (InputManager::isKeyDown(Key::TWO))
            {
                ParticleSystem::update();
            }
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
        
        fpsCounter.addFrame();
        fpsCounter.update(deltaTime);
        while (accumulator >= targetTickTime)
        {
            ParticleSystem::update();
            accumulator -= targetTickTime;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);         

        ParticleSystem::render(cam);
        glfwSwapBuffers(window);
    }
}

void App::vSyncEnabled(bool enabled)
{
    if (enabled)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }
}

void App::setTargetFPS(int fps)
{
    targetFPS = fps;
    if (fps == 0)
    {
        targetFrameTime = 0.0;
    }
    else
    {
        targetFrameTime = 1.0 / targetFPS;
    }
}

void App::setWindowSize(int width, int height)
{

}
