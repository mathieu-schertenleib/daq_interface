#ifndef LOGGING_HPP
#define LOGGING_HPP

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif
#include <fmt/chrono.h>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include <fmt/color.h>

#include <chrono>
#include <cstdio>

template <typename... T>
inline void log_info(fmt::format_string<T...> fmt, T &&...args)
{
    fmt::print(fg(fmt::color::yellow_green),
               "[{}] [INFO]    ",
               std::chrono::system_clock::now());
    fmt::print(
        fg(fmt::color::yellow_green), fmt.get(), std::forward<T>(args)...);
    fmt::print("\n");
    std::fflush(stdout);
}

template <typename... T>
inline void log_warning(fmt::format_string<T...> fmt, T &&...args)
{
    fmt::print(fg(fmt::color::orange),
               "[{}] [WARNING] ",
               std::chrono::system_clock::now());
    fmt::print(fg(fmt::color::orange), fmt.get(), std::forward<T>(args)...);
    fmt::print("\n");
    std::fflush(stdout);
}

template <typename... T>
inline void log_error(fmt::format_string<T...> fmt, T &&...args)
{
    fmt::print(fg(fmt::color::crimson),
               "[{}] [ERROR]   ",
               std::chrono::system_clock::now());
    fmt::print(fg(fmt::color::crimson), fmt.get(), std::forward<T>(args)...);
    fmt::print("\n");
    std::fflush(stdout);
}

#endif // LOGGING_HPP
