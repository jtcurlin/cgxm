//examples/sandbox/src/sandbox.h

#pragma once

#include <cgxm/core/engine.hpp>

class Sandbox final : public cgxm::core::Engine
{
  public:
    Sandbox() = default;
    ~Sandbox() = default;

    void init() override;
    void run() override;
    void stop() override;
};
