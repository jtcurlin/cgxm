// src/engine.cpp

#include <cgxm/core/engine.h>
#include <cgxm/core/log.hpp>
#include <chrono>
#include <thread>
#include <iostream>

#define SLEEP_DURATION 1000

namespace cgxm::core
{

void Engine::init() 
{
    log::init();
    LOG_TRACE("initialized");
}

void Engine::run() 
{
    for(int i=0; i<4; i++)
    {
        LOG_TRACE("iteration #{}", i);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION));
    }
}

void Engine::stop() 
{
    LOG_TRACE("shutting down");
}

}
