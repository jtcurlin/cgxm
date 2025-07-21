// src/platform/app.hpp

#pragma once
#include <string>

enum class AppState
{
    Startup,
    Running,
    Shutdown,
    Stopped
};

namespace cgxm::platform
{
class Window;
struct WindowDesc;

struct AppDesc
{
    std::string name;
};

class App
{
public:
    explicit App(const AppDesc& d);
    ~App();

    bool init();
    void poll();
    bool shutdown();

    void set_state(AppState state);
    AppState get_state() const;

    uint16_t create_window(const WindowDesc& d);
    Window* get_window(uint16_t id);
    bool destroy_window(uint16_t id);

private:
    std::string m_name;
    AppState m_state{AppState::Startup};

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace cgxm::platform