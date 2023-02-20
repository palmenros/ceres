#ifndef COMPILER_LOG_HPP
#define COMPILER_LOG_HPP

#include "spdlog/spdlog.h"

namespace Ceres::Log {

// Set up spdlog for logging
inline void setupLogging()
{
    spdlog::set_pattern("%^[internal %l]%$ %v");
    // TODO: Update the level when needed
    spdlog::set_level(spdlog::level::trace);
}

// Just wrappers to call spdlog

template<typename... Args> inline void debug(Args&&... args) { spdlog::debug(std::forward<Args>(args)...); }

template<typename... Args> inline void info(Args&&... args) { spdlog::info(std::forward<Args>(args)...); }

template<typename... Args> inline void warn(Args&&... args) { spdlog::warn(std::forward<Args>(args)...); }

template<typename... Args> inline void error(Args&&... args) { spdlog::error(std::forward<Args>(args)...); }

template<typename... Args> inline void critical(Args&&... args) { spdlog::critical(std::forward<Args>(args)...); }

// Panic support

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#    define CERES_PRETTY_FUNCTION_NAME __FUNCSIG__
#else
#    define CERES_PRETTY_FUNCTION_NAME __PRETTY_FUNCTION__
#endif

template<typename... Args> [[noreturn]] inline void panic(Args&&... args)
{
    critical(std::forward<Args>(args)...);
    exit(1);
}

// Macro for NOT_IMPLEMENTED
#define NOT_IMPLEMENTED() \
    ::Ceres::Log::panic("Not implemented: {} in {}, line {}", CERES_PRETTY_FUNCTION_NAME, __FILE__, __LINE__)
#define TODO() \
    ::Ceres::Log::panic("TODO: Not implemented: {} in {}, line {}", CERES_PRETTY_FUNCTION_NAME, __FILE__, __LINE__)

// TODO: Maybe define ASSERT only on DEBUG
#define ASSERT(cond)                                                                                            \
    do {                                                                                                        \
        if (!(cond)) {                                                                                          \
            ::Ceres::Log::panic("Assertion '{}' failed in {}, line {}, function {}", #cond, __FILE__, __LINE__, \
                CERES_PRETTY_FUNCTION_NAME);                                                                    \
        }                                                                                                       \
    } while (0)

} // namespace Ceres::Log

#endif // COMPILER_LOG_HPP
