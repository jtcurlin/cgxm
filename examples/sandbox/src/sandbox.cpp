#include "sandbox.h"
#include <iostream>

void Sandbox::init()
{
  Engine::init();
  std::cout << "Sandbox initialized" << std::endl;
}

void Sandbox::run()
{
  Engine::run();
  std::cout << "Sandbox run" << std::endl;
}

void Sandbox::stop()
{
  Engine::stop();
  std::cout << "Sandbox stop" << std::endl;
}

