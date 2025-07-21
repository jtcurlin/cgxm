// src/platform/darwin/app.mm

#include "../app.hpp"
#include "../window.hpp"
#include <cgxm/core/log.hpp>

#include <vector>

#import <AppKit/AppKit.h>

@interface AppDelegate : NSObject<NSApplicationDelegate>
- (instancetype) initWithCGXMApp:(cgxm::platform::App*)app;
@end

@implementation AppDelegate
{
    cgxm::platform::App* cgxm_app;
}

- (instancetype)initWithCGXMApp:(cgxm::platform::App*)app
{
    if ((self = [super init]))
    {
        cgxm_app = app;
    }
    return self;
}

- (void) applicationDidFinishLaunching:(NSNotification*)notification
{
    LOG_INFO("applicationDidFinishLaunching");
    cgxm_app->set_state(AppState::Running);
}

@end

namespace cgxm::platform
{

struct App::Impl
{
    id<NSApplicationDelegate> app_delegate = nil;
    id<MTLDevice> device = nil; // should app own this or renderer?

    std::vector<std::unique_ptr<Window>> windows;
    std::vector<uint16_t> free_window_ids;
};


App::App(const AppDesc& d)
    : m_name(d.name),
      m_impl(std::make_unique<Impl>()) {}

App::~App() = default;

bool App::init()
{
    [NSApplication sharedApplication];
    m_impl->app_delegate = [[AppDelegate alloc] initWithCGXMApp:this];
    [NSApp setDelegate:m_impl->app_delegate];
    [NSApp finishLaunching];
    // [NSApp run];
    return true;
}

void App::poll()
{
    while (true)
    {
        NSEvent* e = [NSApp nextEventMatchingMask:NSEventMaskAny
                                  untilDate:[NSDate distantPast]
                                    inMode:NSDefaultRunLoopMode
                                       dequeue:YES];
        if (!e) break;
        [NSApp sendEvent:e];
    }
    [NSApp updateWindows];
}

bool App::shutdown()
{
    // todo
    LOG_INFO("app::shutdown()");
    return true;
}

void App::set_state(const AppState state)
{
    m_state = state;
}

AppState App::get_state() const
{
    return m_state;
}

uint16_t App::create_window(const WindowDesc& d)
{
    auto new_window = std::make_unique<Window>(d);
    if (!new_window->init())
    {
        LOG_ERROR("Failed to initialize window");
        return UINT16_MAX;
    }

    uint16_t id;
    if (!m_impl->free_window_ids.empty())
    {
        id = m_impl->free_window_ids.back();
        m_impl->free_window_ids.pop_back();
        m_impl->windows[id] = std::move(new_window);
    }
    else
    {
        id = static_cast<uint16_t>(m_impl->windows.size());
        m_impl->windows.push_back(std::move(new_window));
    }
    LOG_INFO("Window #{} successfully created & initialized", id);

    return id;
}

Window* App::get_window(uint16_t id)
{
    return m_impl->windows[id].get();
}


bool App::destroy_window(uint16_t id)
{
    if (id >= m_impl->windows.size() || !m_impl->windows[id])
    {
        LOG_ERROR("Window #{} does not exist, failed to destroy", id);
        return false;
    }

    if (m_impl->windows[id]->shutdown())
    {
        LOG_ERROR("Window #{} failed to shutdown", id);
        return false;
    }
    m_impl->windows[id].reset();

    LOG_INFO("Window #{} successfully shutdown & destroyed", id);
    return true;
}

} // namespace cgxm::platform
