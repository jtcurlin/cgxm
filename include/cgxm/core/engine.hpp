#pragma once
#include <memory>

namespace cgxm::platform
{
    class App;
}

namespace cgxm::core
{

class Engine {
  public:
    Engine();
    ~Engine();

    virtual void init();
    virtual void run();
    virtual void stop();

  private:
    // bool                      m_is_running{false};
    std::unique_ptr<platform::App> m_app;

};

}
