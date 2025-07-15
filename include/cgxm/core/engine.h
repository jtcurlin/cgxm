#pragma once

namespace cgxm::core
{

class Engine {
  public:
    Engine() = default;
    ~Engine() = default;

    virtual void init();
    virtual void run();
    virtual void stop();

  private:
    bool m_is_running{false};
};

}
