// src/core/engine.cpp

#include <chrono>
#include <thread>
#include <iostream>

#include <cgxm/core/engine.hpp>
#include <cgxm/core/log.hpp>
#include "platform/app.hpp"
#include "platform/window.hpp"

#define SLEEP_DURATION 1000

namespace cgxm::core
{

Engine::Engine() = default;
Engine::~Engine() = default;

void Engine::init()
{
    log::init();
    LOG_TRACE("initialized");

    m_app = std::make_unique<platform::App>(platform::AppDesc{"cgxm-app"});
    m_app->init();
    while (m_app->get_state() != AppState::Running) {
        m_app->poll();
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION));
    }
    const platform::WindowDesc window_desc{"cgxm", 1920, 1080, 0, 0};
    const auto main_wid = m_app->create_window(window_desc);
    m_app->get_window(main_wid)->focus();
}

void Engine::run() 
{
    for(int i=0; i<10; i++)
    {
        LOG_TRACE("iteration #{}", i);
        m_app->poll();
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION));
    }
}

void Engine::stop() 
{
    LOG_TRACE("shutting down");
}

}
