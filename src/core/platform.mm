// src/core/platform.mm

#include <vector>

#include <cgxm/core/platform.hpp>
#include <cgxm/core/log.hpp>

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

namespace cgxm::core
{
    struct Platform::Impl
    {
        id<NSApplicationDelegate> m_app_delegate = nil;
        std::vector<void*> m_windows;

        ~Impl()
        {
            for (void* w_ptr : m_windows)
            {
                NSWindow* w = (__bridge_transfer NSWindow*)w_ptr;
                (void)w;
            }
        }
    };
}

@interface AppDelegate : NSObject <NSApplicationDelegate>
- (instancetype)initWithPlatform:(cgxm::core::Platform*)platform;
@end

@implementation AppDelegate
{
  cgxm::core::Platform* m_platform;
}

- (instancetype) initWithPlatform:(cgxm::core::Platform*)platform {
     if ((self = [super init])) {
           m_platform = platform;
     }
     return self;
}

- (void) applicationDidFinishLaunching:(NSNotification *) notification
{
     m_platform->create_window(cgxm::core::window_desc{});
}
@end

namespace cgxm::core
{

Platform::Platform() : p_(std::make_unique<Impl>()) {}
Platform::~Platform() = default;

void Platform::init()
{
  @autoreleasepool {
      [NSApplication sharedApplication];
      p_->m_app_delegate = [[AppDelegate alloc] initWithPlatform:this];
      [NSApp setDelegate:p_->m_app_delegate];
      [NSApp finishLaunching];
      // [NSApp run];
  }

  LOG_INFO("Initialized");
}

void Platform::poll()
{
    LOG_INFO("Draining appkit events");
    @autoreleasepool
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
}


void Platform::create_window(window_desc d)
{
    @autoreleasepool {
        NSRect rect = NSMakeRect(d.x, d.y, d.w, d.h);

        NSWindow* win = [[NSWindow alloc]
            initWithContentRect:rect
            styleMask:NSWindowStyleMaskTitled |
                     NSWindowStyleMaskClosable |
                     NSWindowStyleMaskResizable
            backing:NSBackingStoreBuffered
              defer:NO];

        NSView* content = [[NSView alloc] initWithFrame:rect];
        [win setContentView:content];
        [win makeKeyAndOrderFront:nil];

        p_->m_windows.push_back((__bridge_retained void*)win);

        NSTextField* my_text_field = [NSTextField labelWithString:@"Hello World"];
        [content addSubview:my_text_field];
    }
}
} // namespace cgxm::core
