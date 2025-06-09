#include "app.h"
#include "graphics/textureloader.h"
#include "utils/globals.h"
#include "utils/logger.h"
#include "input/inputmanager.h"
#include "input/mouse.h"

App::App()
{
    if (initializeWindow())
    {
        LOG_INFO << "App initialized";
        appError = false;
    }
    else
    {
        LOG_ERROR << "Error during app initialization";
        appError = true;
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
    window = glfwCreateWindow(600, 400, "window", NULL, NULL);

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                   {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
                                    
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
    unsigned char *icon_data = TextureLoader::loadTexture("assets/thumbnail.png", &x, &y, &n);

    if (icon_data)
    {
        GLFWimage img{img.width = x, img.height = y, img.pixels = icon_data};
        glfwSetWindowIcon(window, 1, &img);
    }
    else
    {
        LOG_WARN << "icon loading failed";
    }
    TextureLoader::free(icon_data);
    //glViewport(0, 0, windowWidth, windowHeight);
    return true;
}

void App::run()
{
    if (appError) return;
    init();
    double lastTickTime = glfwGetTime();
    double lastFrameTime = glfwGetTime();
    double accumulator = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        double now = glfwGetTime();
        double deltaTime = now - lastTickTime;
        lastTickTime = now;
        accumulator += deltaTime;
        
        InputManager::update(window);
        Mouse::update(window, cam);
        gui.update();
        handleInput();

        fpsCounter.addFrame();
        fpsCounter.update(deltaTime);
        while (accumulator >= targetTickTime)
        {
            particleSystem.update();
            accumulator -= targetTickTime;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        render();
        glfwSwapBuffers(window);
    }
}

void App::init()
{

    cam.position = glm::vec2(0, 0);
    particleSystem.init();
    Mouse::init(window);
    gui.init();
    textRenderer.init();
}

void App::handleInput()
{
    if (!gui.isMouseOverUI())
    {
        if (InputManager::isKeyDown(Btn::LEFT))
        {
            particleSystem.spawnParticles(Mouse::getWorldMousePos(), ParticleType::Sand, 5);
        }
        if (InputManager::isKeyDown(Key::ONE))
        {
            particleSystem.spawnParticles(Mouse::getWorldMousePos(), ParticleType::Water, 5);
        }
        if (InputManager::isKeyDown(Key::THREE))
        {
            particleSystem.spawnParticles(Mouse::getWorldMousePos(), ParticleType::Smoke, 5);
        }
        if (InputManager::isKeyDown(Key::FOUR))
        {
            particleSystem.spawnParticles(Mouse::getWorldMousePos(), ParticleType::Stone, 2);
        }
    }
    if (InputManager::isKeyDown(Btn::RIGHT))
    {
        cam.position -= Mouse::mouseDelta * cam.zoom;
    }
    if (InputManager::isKeyJustPressed(Key::P))
    {
        LOG_INFO << "Particle count: " << particleSystem.particleCount;
    }
    if (InputManager::isKeyJustPressed(Key::R))
    {
        cam.position = glm::vec2(0.0f, 0.0f);
        cam.zoom = 1.0f;
        cam.rotation = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void App::render()
{
    particleSystem.render(cam);
    gui.render(cam);
    textRenderer.RenderText(cam, "Fps: " + std::to_string(fpsCounter.fps), 
        0, 0, 
        1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    auto pos = Mouse::getWorldMousePos();
    textRenderer.RenderText(cam, "Mouse pos: " + std::to_string(pos.x) + " " + std::to_string(pos.y),
        0, textRenderer.fontSize,
        1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    textRenderer.RenderText(cam, "Particles: " + std::to_string(particleSystem.particleCount),
        0, textRenderer.fontSize * 2,
        1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
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
	glfwSetWindowSize(window, width, height);
}