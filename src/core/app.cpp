#include "app.h"
#include "graphics/textureloader.h"
#include "graphics/textrenderer.h"
#include "utils/globals.h"
#include "utils/fpscounter.h"
#include "utils/logger.h"
#include "input/inputmanager.h"
#include "input/mouse.h"
#include "particles/particlesystem.h"

App::App()
{
    if (initializeWindow())
    {
        LOG_INFO << "App initialized";
    }
    else
    {
        LOG_ERROR << "Error during app initialization";
    }
}

App::~App()
{
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
    LOG_INFO << "App destroyed";
}

bool App::initializeWindow()
{
    if (!glfwInit())
    {
        LOG_ERROR << "glfw init error";
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
        glViewport(0, 0, width, height); });
    glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset)
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
            } });
    glfwSetWindowSizeLimits(window, 320, 180, 7680, 4320);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR << "glad init error";
        glfwTerminate();
        return false;
    }

    int x, y, n;
    unsigned char *data = TextureLoader::loadTexture("assets/thumbnail.png", &x, &y, &n);

    if (data)
    {
        GLFWimage img{img.width = x, img.height = y, img.pixels = data};
        glfwSetWindowIcon(window, 1, &img);
    }
    else
    {
        LOG_WARN << "icon loading failed";
    }
    TextureLoader::free(data);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    return true;
}

void App::run()
{
    cam.viewportWidth = WINDOW_WIDTH;
    cam.viewportHeight = WINDOW_HEIGHT;
    cam.position = glm::vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    ParticleSystem::init();
    Mouse::init(window);
    gui.init();

    FPSCounter fpsCounter;
    double lastTickTime = glfwGetTime();
    double lastFrameTime = glfwGetTime();
    double accumulator = 0.0;
    bool step = false;

    TextRenderer t;
    t.init();

    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        double deltaTime = now - lastTickTime;
        lastTickTime = now;
        accumulator += deltaTime;

        glfwPollEvents();
        InputManager::update(window);
        Mouse::update(window, cam);
        gui.update();
        if (!gui.isMouseOverUI())
        {
            if (InputManager::isKeyDown(Btn::LEFT))
            {
                ParticleSystem::spawnParticles(Mouse::getWorldMousePos(), ParticleType::Sand, 5);
            }
            if (InputManager::isKeyDown(Key::ONE))
            {
                ParticleSystem::spawnParticles(Mouse::getWorldMousePos(), ParticleType::Water, 5);
            }
            if (InputManager::isKeyDown(Key::THREE))
            {
                ParticleSystem::spawnParticles(Mouse::getWorldMousePos(), ParticleType::Smoke, 5);
            }
        }
        if (InputManager::isKeyDown(Btn::RIGHT))
        {
            cam.position -= Mouse::mouseDelta * cam.zoom;
        }
        if (InputManager::isKeyJustPressed(Key::P))
        {
            LOG_INFO << "Particle count: " << ParticleSystem::particleCount;
        }
        if (InputManager::isKeyJustPressed(Key::Z))
        {
            step = !step;
        }
        if (InputManager::isKeyJustPressed(Key::R))
        {
            cam.position = glm::vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
            cam.zoom = 1.0f;
            cam.rotation = 0.0f;
        }
        if (step || true)
        {
            if (InputManager::isKeyJustPressed(Key::TWO))
            {
                auto time = glfwGetTime();
                ParticleSystem::update();
                LOG_INFO << "Time: " << glfwGetTime() - time;
            }
        }

        fpsCounter.addFrame();
        fpsCounter.update(deltaTime);
        while (accumulator >= targetTickTime)
        {
            if (!step)
            {
                ParticleSystem::update();
            }
            accumulator -= targetTickTime;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ParticleSystem::render(cam);
        gui.render(cam);
		t.RenderText(cam, "Fps: " + std::to_string(fpsCounter.fps), 0.0f, cam.viewportHeight - 48, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        glfwSwapBuffers(window);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
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

void App::setTargetTicks(int ticks)
{
    targetTicks = ticks;
    targetTickTime = 1.0 / targetTicks;
}

void App::setWindowSize(int width, int height)
{
}
