#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>

template <typename T>
void log(const T &arg)
{
  std::cout << arg;
}

template <typename T, typename... Args>
void log(const T &firstArg, const Args &...args)
{
  std::cout << firstArg;
  log(args...);
}

// Macro for debug messages
#define LOG_DEBUG(...)     \
  std::cout << "[DEBUG] "; \
  log(__VA_ARGS__);        \
  std::cout << std::endl;

// Macro for fatal messages
#define LOG_FATAL(...)       \
  do                         \
  {                          \
    std::cerr << "[FATAL] "; \
    log(__VA_ARGS__);        \
    std::cerr << std::endl;  \
    std::terminate();        \
  } while (0)

// Macro for error messages
#define LOG_ERROR(...)     \
  std::cerr << "[ERROR] "; \
  log(__VA_ARGS__);        \
  std::cerr << std::endl;

// Macro for warning messages
#define LOG_WARN(...)     \
  std::cerr << "[WARN] "; \
  log(__VA_ARGS__);       \
  std::cerr << std::endl;

#define CORE_WARN(condition, ...) \
  if (condition)                  \
  {                               \
    LOG_WARN(__VA_ARGS__);        \
  }

#define CORE_ERROR(condition, ...) \
  if (condition)                   \
  {                                \
    LOG_ERROR(__VA_ARGS__);        \
  }

#define CORE_FATAL(condition, ...) \
  if (condition)                   \
  {                                \
    LOG_FATAL(__VA_ARGS__);        \
  }

#endif