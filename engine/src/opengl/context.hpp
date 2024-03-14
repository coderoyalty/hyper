#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#ifndef GL_CONTEXT_HPP
#define GL_CONTEXT_HPP

namespace hyp {
	class OpenglContext {
	public:
		OpenglContext(GLFWwindow* window);
	private:
		void init();
		void swapBuffer();
	private:
		GLFWwindow* m_window;

		friend class Window;
	};
}

#endif