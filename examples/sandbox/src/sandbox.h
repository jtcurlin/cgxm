#pragma once

#include "core/engine.h"

class Sandbox final : public cgxm::core::Engine
{
  public:
    Sandbox() = default;
    ~Sandbox() = default;

    void init() override;
    void run() override;
    void stop() override;
};
