#include <cgxm/core/log.hpp>

#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/sinks/ConsoleSink.h"

namespace cgxm::log
{

void init()
{
    // 1. Start the backend thread as specified in the official README
    quill::Backend::start();

    // 2. Create the logger. This will be retrieved by our get_logger() helper.
    quill::Logger* logger = quill::Frontend::create_or_get_logger(
      "root", quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_id_1"));

    logger->set_log_level(quill::LogLevel::TraceL3);
}

quill::Logger* get_logger()
{
    // Retrieve the logger created during initialization by its name
    return quill::Frontend::get_logger("root");
}

}