#include "CoreMain.h"

#include <locale>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Graphics/API/GL/GL46/GL46Renderer.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGConsole/API/Console.h"
#include "SGCore/Utils/ShadersPaths.h"

#include "SGCore/Input/InputManager.h"

#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Physics/PhysicsWorld3D.h"
#include "SGCore/UI/FontsManager.h"

void SGCore::CoreMain::start()
{
    const auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream timeStringStream;
    timeStringStream << std::put_time(std::localtime(&in_time_t), "%Y_%m_%d_%H_%M_%S");

    auto currentSessionLogger = spdlog::basic_logger_mt("current_session", "logs/sg_log_" + timeStringStream.str() + ".txt");
    spdlog::set_default_logger(currentSessionLogger);

    // todo: move
    /*system("chcp 65001");
    setlocale(LC_ALL, "Russian");*/

    m_renderer = GL4Renderer::getInstance();
    // m_renderer = GL46Renderer::getInstance();
    //m_renderer = VkRenderer::getInstance();

    m_window.create();

    m_renderer->init();

    SGUtils::Singleton::getSharedPtrInstance<ShadersPaths>()->createDefaultPaths();
    InputManager::init();
    AssetManager::init();
    FontsManager::init();

    Ref<TimerCallback> globalTimerCallback = MakeRef<TimerCallback>();

    // update
    globalTimerCallback->setUpdateFunction([](const double& dt)
                                                {
                                                    update(dt);
                                                });
    
    Ref<TimerCallback> fpsTimerCallback = MakeRef<TimerCallback>();
    
    m_renderTimer.addCallback(globalTimerCallback);
    m_renderTimer.setTargetFrameRate(1200.0);
    m_renderTimer.m_useFixedUpdateCatchUp = false;

    // -----------------
    
    std::shared_ptr<TimerCallback> fixedTimerCallback = std::make_shared<TimerCallback>();

    fixedTimerCallback->setFixedUpdateFunction([](const double& dt, const double& fixedDt)
                                                {
                                                    fixedUpdate(dt, fixedDt);
                                                });

    m_fixedTimer.addCallback(fixedTimerCallback);

    //Graphics::GL::GL4Renderer::getInstance()->checkForErrors();

    sgCallCoreInitCallback();
    
    m_fixedTimer.resetTimer();
    m_renderTimer.resetTimer();

    double cur = glfwGetTime();
    double t = 0;
    int f = 0;
    
    while(!m_window.shouldClose())
    {
        m_renderTimer.startFrame();
        m_fixedTimer.startFrame();
    }
}

void SGCore::CoreMain::fixedUpdate(const double& dt, const double& fixedDt)
{
    InputManager::startFrame();

    sgCallFixedUpdateCallback(dt, fixedDt);
}

void SGCore::CoreMain::update(const double& dt)
{
    glm::ivec2 windowSize;
    m_window.getSize(windowSize.x, windowSize.y);
    m_renderer->prepareFrame(windowSize);

    sgCallUpdateCallback(dt);

    m_window.swapBuffers();

    m_window.pollEvents();
}

SGCore::Window& SGCore::CoreMain::getWindow() noexcept
{
    return m_window;
}

SGCore::Ref<SGCore::IRenderer> SGCore::CoreMain::getRenderer() noexcept
{
    return m_renderer;
}

SGCore::Timer& SGCore::CoreMain::getRenderTimer() noexcept
{
    return m_renderTimer;
}

SGCore::Timer& SGCore::CoreMain::getFixedTimer() noexcept
{
    return m_fixedTimer;
}

std::uint16_t SGCore::CoreMain::getFPS() noexcept
{
    return m_renderTimer.getFramesPerSecond();
}
