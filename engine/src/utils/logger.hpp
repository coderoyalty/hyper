#pragma once
#ifndef HYPER_LOGGER_HPP
#define HYPER_LOGGER_HPP

#include <string>
#include <cstdarg>
#include <sstream>
#include "core/base.hpp"
#include <windows.h>
#include <string>
#include <stdio.h>
#include <cstdint>
#include "utils/assert.hpp"

namespace hyp
{
  enum LOG_TYPE
  {
    FATAL,
    H_ERROR,
    WARN,
    INFO,
    DEBUG,
    TRACE
  };

  template <typename... Types>
  void log_output(hyp::LOG_TYPE type, const std::string &message, Types... args)
  {
    HANDLE stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (type)
    {
    case hyp::LOG_TYPE::DEBUG:
    {
      SetConsoleTextAttribute(stdOutHandle, 5);
      std::stringstream ss;
      ss << "[DEBUG]: " << message << "\n";
      printf(ss.str().c_str(), args...);
    }
    break;
    case hyp::LOG_TYPE::WARN:
    {
      SetConsoleTextAttribute(stdOutHandle, 6);
      std::stringstream ss;
      ss << "[WARN]:  " << message << "\n";
      printf(ss.str().c_str(), args...);
    }
    break;
    case hyp::LOG_TYPE::FATAL:
    {
      SetConsoleTextAttribute(stdOutHandle, 4);
      std::stringstream ss;
      ss << "[FATAL]: " << message << "\n";
      printf(ss.str().c_str(), args...);
    }
    break;
    case hyp::LOG_TYPE::TRACE:
    {
      SetConsoleTextAttribute(stdOutHandle, 2);
      std::stringstream ss;
      ss << "[TRACE]: " << message << "\n";
      printf(ss.str().c_str(), args...);
    }
    break;
    case hyp::LOG_TYPE::INFO:
    {
      SetConsoleTextAttribute(stdOutHandle, 10);
      std::stringstream ss;
      ss << "[INFO]:  " << message << "\n";
      printf(ss.str().c_str(), args...);
    }
    break;
    case hyp::LOG_TYPE::H_ERROR:
    {
      SetConsoleTextAttribute(stdOutHandle, 4);
      std::stringstream ss;
      ss << "[ERROR]: " << message << "\n";
      printf(ss.str().c_str(), args...);
    }
    break;
    default:
      break;
    }

    SetConsoleTextAttribute(stdOutHandle, 7);
  }

#if defined(HYPER_DEBUG)
#define HYP_WARN(message, ...) log_output(hyp::LOG_TYPE::WARN, message, ##__VA_ARGS__);
#define HYP_INFO(message, ...) log_output(hyp::LOG_TYPE::INFO, message, ##__VA_ARGS__);
#define HYP_DEBUG(message, ...) log_output(hyp::LOG_TYPE::DEBUG, message, ##__VA_ARGS__);
#define HYP_FATAL(message, ...) log_output(hyp::LOG_TYPE::FATAL, message, ##__VA_ARGS__);
#define HYP_ERROR(message, ...) log_output(hyp::LOG_TYPE::H_ERROR, message, ##__VA_ARGS__);
#define HYP_TRACE(message, ...) log_output(hyp::LOG_TYPE::TRACE, message, ##__VA_ARGS__);
#else

#define HYP_WARN(message, ...)
#define HYP_INFO(message, ...)
#define HYP_DEBUG(message, ...)
#define HYP_FATAL(message, ...)
#define HYP_ERROR(message, ...)
#define HYP_TRACE(message, ...)

#endif
}

#endif