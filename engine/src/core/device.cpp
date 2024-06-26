#include "pch.h"
#include "device.hpp"
#include <GLFW/glfw3.h>
#include "utils/logger.hpp"
#include <core/version.hpp>
#include <utils/console.hpp>

void hyp::Device::init(const DeviceSetting& ds) {
	if (ds.majorVersion < 3)
	{
		HYP_ERROR("The OpenGL version used it not supported");
		std::terminate();
	}

	int code = glfwInit();
	if (!code)
	{
		HYP_ERROR("GLFW API initialized failed");
		std::terminate();
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ds.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ds.majorVersion);
	glfwWindowHint(GLFW_SAMPLES, ds.samples);
#ifdef HYPER_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif // HYP_DEBUG

	HYP_INFO("OpenGL Version %.d.%.d", ds.majorVersion, ds.minorVersion);
	HYP_INFO("Hyper Version %d.%d.%d", HYPER_VERSION_MAJOR, HYPER_VERSION_MINOR, HYPER_VERSION_PATCH);

	CONSOLE_LOG("OpenGL Version %.d.%.d", ds.majorVersion, ds.minorVersion);
	CONSOLE_LOG("Hyper Version %d.%d.%d", HYPER_VERSION_MAJOR, HYPER_VERSION_MINOR, HYPER_VERSION_PATCH);
}

void hyp::Device::deinit() {
	glfwTerminate();
	HYP_INFO("GLFW API terminated successfully.");
}
