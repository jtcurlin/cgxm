#pragma once
#include <memory>

namespace cgxm::core
{

class Platform;

class Engine {
  public:
    Engine();
    ~Engine();

    virtual void init();
    virtual void run();
    virtual void stop();

  private:
    // bool                      m_is_running{false};
    std::unique_ptr<Platform> m_platform;

};

}
