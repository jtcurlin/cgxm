// src/engine.cpp

#include "core/engine.h"
#include <chrono>
#include <thread>
#include <iostream>

#define SLEEP_DURATION 1000

namespace cgxm::core
{

void Engine::init() 
{
  std::cout << "[cgxm] Engine::start()" << std::endl;
}

void Engine::run() 
{
    for(int i=0; i<4; i++)
    {
        std::cout << "[cgxm] Engine::run() -- iteration " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION));
    }
}

void Engine::stop() 
{
  std::cout << "[cgxm] Engine::stop()" << std::endl;
}

}
