#include "Logger.hpp"

namespace Agina 
{
    std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;

    void Logger::Init()
    {
        // Set pattern: [Time] Name: Message (with colors)
        spdlog::set_pattern("%^[%T] %n: %v%$"); 
        s_CoreLogger = spdlog::stdout_color_mt("AGINA");
        s_CoreLogger->set_level(spdlog::level::trace);
    }
}
