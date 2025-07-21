// src/platform/darwin/window.mm

#include "../window.hpp"

#import <AppKit/AppKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>

namespace cgxm::platform
{

struct Window::Impl
{
    NSWindow* ns_window = nil;
    NSView* ns_cview = nil;
    CAMetalLayer* ca_mlayer = nil;
};

Window::Window(const WindowDesc& d)
    : m_title(d.title),
      m_width(d.width),
      m_height(d.height),
      m_xpos(d.xpos),
      m_ypos(d.ypos),
      m_impl(std::make_unique<Impl>()) {}

Window::~Window() = default;

bool Window::init()
{
    NSRect content_rect = NSMakeRect(m_xpos, m_ypos, m_width, m_height);

    // initialize window
    m_impl->ns_window = [[NSWindow alloc]
            initWithContentRect:content_rect
            styleMask:NSWindowStyleMaskTitled |
                     NSWindowStyleMaskClosable |
                     NSWindowStyleMaskResizable
            backing:NSBackingStoreBuffered
              defer:NO];

    // set / initialize window title
    NSString* title = [NSString stringWithUTF8String:m_title.c_str()];
    [m_impl->ns_window setTitle:title];

    // initialize view
    m_impl->ns_cview = [[NSView alloc] initWithFrame:content_rect];
    [m_impl->ns_window setContentView:m_impl->ns_cview];

    return true; // no errors, successfully intiialized
}

bool Window::shutdown()
{
    // todo
    return true;
}

void Window::focus()
{
    [m_impl->ns_window makeKeyAndOrderFront:nil];
}

} // namespace cgxm::platform

@interface MetalView : NSView
@end

@implementation MetalView
- (instancetype) initWithFrame:(NSRect)frame
{
    if ((self = [super initWithFrame:frame]))
    {
        // 'layer-hosting'
        CAMetalLayer* camLayer = [CAMetalLayer layer];
        [self setLayer:camLayer];

        /* wantsLayer docs explicitly say we need to set this, after
         * our setLayer call, in order to configure layer-hosting view.
         * however it seems this is implicitly done by setLayer, making this
         * redundant?
         */
        // [self setWantsLayer:YES];

        [camLayer setDevice:MTLCreateSystemDefaultDevice()];
        [camLayer setPixelFormat:MTLPixelFormatBGRA8Unorm];
        [camLayer setFramebufferOnly:YES];
        // [camLayer setContentsScale:[[NSScreen mainScreen] backingScaleFactor]];
    }
    return self;
}

/*
// called only by UIKit (iOS)
+ (Class) layerClass
{
    return [CAMetalLayer class];
}

// called only by AppKit (MacOS)
- (CALayer *) makeBackingLayer
{
    return [CAMetalLayer layer];
}
*/

@end


