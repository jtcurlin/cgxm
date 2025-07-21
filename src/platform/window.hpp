// src/platform/window.hpp

#pragma once
#include <memory>
#include <string>

namespace cgxm::platform
{
struct WindowDesc
{
    std::string title;
    uint16_t width;
    uint16_t height;
    uint16_t xpos;
    uint16_t ypos;
};

class Window
{
  public:
    explicit Window(const WindowDesc& d);
    ~Window();

    bool init();
    bool shutdown();

    void focus();

  private:
    std::string m_title;
    uint16_t m_width;
    uint16_t m_height;
    uint16_t m_xpos;
    uint16_t m_ypos;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace cgxm::platform