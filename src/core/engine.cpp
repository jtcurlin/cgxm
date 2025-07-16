// src/core/engine.cpp

#include <chrono>
#include <thread>
#include <iostream>

#include <cgxm/core/engine.hpp>
#include <cgxm/core/log.hpp>
#include <cgxm/core/platform.hpp>

#define SLEEP_DURATION 1000

namespace cgxm::core
{

Engine::Engine() = default;

Engine::~Engine() = default;

void Engine::init()
{
    log::init();
    LOG_TRACE("initialized");

    m_platform = std::make_unique<Platform>();
    m_platform->init();
}

void Engine::run() 
{
    for(int i=0; i<10; i++)
    {
        LOG_TRACE("iteration #{}", i);
        m_platform->poll();
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION));
    }
}

void Engine::stop() 
{
    LOG_TRACE("shutting down");
}

}
