#pragma once

#ifndef HYPER_DEVICE_HPP
	#define HYPER_DEVICE_HPP
	#include <glad/glad.h>
	#include <core/base.hpp>
	#include <system/export.hpp>

namespace hyp {
	struct HYPER_API DeviceSetting
	{
		int8_t majorVersion = 3;
		int8_t minorVersion = 3;
		int8_t samples = 4;
	};
	class HYPER_API Device {
	public:
		static void init(const DeviceSetting& ds);
		static void deinit();
	};
}

#endif
