// include/cgxm/render/renderer.hpp

#pragma once
#include <cstdint>
#include <span>

namespace cgxm::render
{
    struct InitDesc
    {
        void* ca_layer;
    };

    struct RenderCommand
    {
        uint32_t mesh;
        uint32_t material;
        uint32_t transform;
    };

    class Renderer
    {
    public:
        static Renderer* create(const InitDesc& desc);
        void destroy();

        void begin_frame(float dt);
        void submit(std::span<const RenderCommand> cmds);
        void end_frame();

    private:
        Renderer();
        void init(const InitDesc& desc);
        struct Impl;    // fwd decl.
        std::unique_ptr<Impl> p_;
    };
} // namespace cgxm::render
