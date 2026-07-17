#pragma once
#include <spdlog/spdlog.h>
#include <memory>
#include <string>

namespace Agina
{
    class Logger
    {
    public:
        static void InitEngineLogger();
        static void InitClientLogger(const std::string& appName);

        static std::shared_ptr<spdlog::logger>& GetEngineLogger();
        static std::shared_ptr<spdlog::logger>& GetClientLogger();

    private:
        static std::shared_ptr<spdlog::logger> s_EngineLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

#define AG_CORE_TRACE(...) ::Agina::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define AG_CORE_INFO(...)  ::Agina::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define AG_CORE_WARN(...)  ::Agina::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define AG_CORE_ERROR(...) ::Agina::Logger::GetEngineLogger()->error(__VA_ARGS__)

#define AG_TRACE(...) ::Agina::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define AG_INFO(...)  ::Agina::Logger::GetClientLogger()->info(__VA_ARGS__)
#define AG_WARN(...)  ::Agina::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define AG_ERROR(...) ::Agina::Logger::GetClientLogger()->error(__VA_ARGS__)
}