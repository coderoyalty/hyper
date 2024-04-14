#include <opengl/context.hpp>
#include <utils/assert.hpp>

hyp::OpenglContext::OpenglContext(GLFWwindow* window) {
	HYP_ASSERT_CORE(window != nullptr, "OpenGL API can't use an invalid GLFW window");
	m_window = window;
}

void hyp::OpenglContext::init() {
	glfwMakeContextCurrent(m_window);
	int val = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	HYP_ASSERT_CORE(val, "Failed to initialize GLAD (graphics API)");
}

void hyp::OpenglContext::swapBuffer() {
	glfwSwapBuffers(m_window);
}
