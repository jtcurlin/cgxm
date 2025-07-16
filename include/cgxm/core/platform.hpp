// include/cgxm/core/platform.hpp

#pragma once
#include <cstdint>
#include <memory>

namespace cgxm::core
{
struct window_desc
{
    uint32_t x{0};
    uint32_t y{0};
    uint32_t w{1920};
    uint32_t h{1080};
};

class Platform
{
    void* m_app_delegate;

public:
    Platform();
    ~Platform();

    void init();
    void poll();
    // void shutdown();

    void create_window(window_desc d);
    // void destroy_window(uint32_t w_id);
    // void get_window(uint32_t w_id);

private:
    struct Impl; // fwd-decl
    std::unique_ptr<Impl> p_;
};
}