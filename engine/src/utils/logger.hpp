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

namespace hyp {
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
	void log_output(hyp::LOG_TYPE type, const std::string& message, Types... args) {
		std::stringstream ss;
		int attribute;
		switch (type)
		{
		case hyp::LOG_TYPE::DEBUG:
		{
			attribute = 5;
			ss << "[DEBUG]: ";
		}
		break;
		case hyp::LOG_TYPE::WARN:
		{
			attribute = 6;
			ss << "[WARN]: ";
		}
		break;
		case hyp::LOG_TYPE::FATAL:
		{
			attribute = 4;
			ss << "[FATAL]: ";
		}
		break;
		case hyp::LOG_TYPE::TRACE:
		{
			attribute = 2;
			ss << "[TRACE]: ";
		}
		break;
		case hyp::LOG_TYPE::INFO:
		{
			attribute = 10;
			ss << "[INFO]:  ";
		}
		break;
		case hyp::LOG_TYPE::H_ERROR:
		{
			attribute = 4;
			ss << "[ERROR]: ";
		}
		break;
		default:
			attribute = 7;
			break;
		}

		ss << message << "\n";
	#ifdef _WIN32
		HANDLE stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(stdOutHandle, attribute);
		printf(ss.str().c_str(), args...);
		SetConsoleTextAttribute(stdOutHandle, 7);
	#else
		printf(ss.str().c_str(), args...);
	#endif
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