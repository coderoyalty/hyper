#ifndef HYP_ASSERT_HPP
	#define HYP_ASSERT_HPP

	#include <cstdint>
	#include <stdio.h>

	#if defined(HYPER_ASSERTION_ENABLED)
		#include <intrin.h>
		#define _DEBUG_BREAK() __debugbreak()
	#else
		#define _DEBUG_BREAK()
	#endif

void report_assert(const char* expr, const char* message, const char* filename, uint32_t line);

	#define HYP_ASSERT(expr)                                  \
		{                                                     \
			if (expr)                                         \
			{                                                 \
			}                                                 \
			else                                              \
			{                                                 \
				report_assert(#expr, "", __FILE__, __LINE__); \
				_DEBUG_BREAK();                               \
			}                                                 \
		}

	#define HYP_ASSERT_CORE(expr, msg, ...)                     \
		{                                                       \
			if (expr)                                           \
			{                                                   \
			}                                                   \
			else                                                \
			{                                                   \
				char temp[256];                                 \
				sprintf_s(temp, 256, msg, ##__VA_ARGS__);       \
				report_assert(#expr, temp, __FILE__, __LINE__); \
				_DEBUG_BREAK();                                 \
			}                                                   \
		}

#else

#endif
