#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace Agina
{
    class Logger 
    {
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
    };
}

// Macros for easy logging
#define AG_CORE_TRACE(...)    ::Agina::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define AG_CORE_INFO(...)     ::Agina::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define AG_CORE_ERROR(...)    ::Agina::Logger::GetCoreLogger()->error(__VA_ARGS__)