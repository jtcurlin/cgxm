#pragma once

#include "quill/LogMacros.h"
#include "quill/Logger.h"

namespace cgxm::log
{
    void init();

    quill::Logger* get_logger();
}

#define LOG_TRACE(...)    QUILL_LOG_TRACE_L3(cgxm::log::get_logger(), __VA_ARGS__)
#define LOG_DEBUG(...)    QUILL_LOG_DEBUG(cgxm::log::get_logger(), __VA_ARGS__)
#define LOG_INFO(...)     QUILL_LOG_INFO(cgxm::log::get_logger(), __VA_ARGS__)
#define LOG_WARN(...)     QUILL_LOG_WARNING(cgxm::log::get_logger(), __VA_ARGS__)
#define LOG_ERROR(...)    QUILL_LOG_ERROR(cgxm::log::get_logger(), __VA_ARGS__)
#define LOG_CRITICAL(...) QUILL_LOG_CRITICAL(cgxm::log::get_logger(), __VA_ARGS__)