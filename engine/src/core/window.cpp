#include "core/window.hpp"
#include <utils/logger.hpp>
#include "timer.hpp"

namespace callback {
	static void registerWindowSize(GLFWwindow* window, int width, int height);
	static void registerWindowClose(GLFWwindow* window);
	static void registerKey(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void registerCursorPos(GLFWwindow* window, double xpos, double ypos);
	static void registerScroll(GLFWwindow* window, double xoffset, double yoffset);
	static void registerMouseBtn(GLFWwindow* window, int button, int action, int mods);
}

hyp::Scope<hyp::Window> hyp::Window::create(WindowProps props) {
	return hyp::CreateScope<hyp::Window>(props);
}

hyp::Window::Window(WindowProps props) : m_props(props) {
	this->init();
}

hyp::Window::~Window() {
	this->close();
	this->deinit();
}

void hyp::Window::setVSync(bool enable) {
	if (enable)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}

	m_props.vsync = enable;
}

void hyp::Window::init() {
	glfwWindowHint(GLFW_RESIZABLE, this->m_props.resizable);
	glfwWindowHint(GLFW_FOCUSED, this->m_props.focus);
	glfwWindowHint(GLFW_VISIBLE, this->m_props.visible);
	glfwWindowHint(GLFW_MAXIMIZED, this->m_props.maximized);

	hyp::WindowData& data = this->m_props.windowData;
	this->m_window = glfwCreateWindow(
	    data.width, data.height, data.title.c_str(),
	    this->m_props.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (!this->m_window)
	{
		HYP_ERROR("Unable to create window");
		throw std::runtime_error("failed to create window");
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	if (mode)
	{
		hyp::Timer::setFPS(mode->refreshRate);
	}

	m_context = hyp::CreateScope<hyp::OpenglContext>(m_window);
	m_context->init();

	if (m_props.maximized)
	{
		int w, h;
		glfwGetWindowSize(m_window, &w, &h);
		data.width = w;
		data.height = h;
	}

	glfwSetWindowUserPointer(m_window, &m_props.windowData);

	glfwSetWindowSizeCallback(m_window, callback::registerWindowSize);
	glfwSetWindowCloseCallback(m_window, callback::registerWindowClose);
	glfwSetKeyCallback(m_window, callback::registerKey);
	glfwSetCursorPosCallback(m_window, callback::registerCursorPos);
	glfwSetScrollCallback(m_window, callback::registerScroll);
	glfwSetMouseButtonCallback(m_window, callback::registerMouseBtn);
}

void hyp::Window::deinit() {
	if (m_window)
	{
		glfwDestroyWindow(m_window);
	}
	return;
}

void hyp::Window::setVisibility(bool visible) {
	glfwSetWindowAttrib(m_window, GLFW_VISIBLE, visible);
	m_props.visible = visible;
}

void hyp::Window::close() {
	glfwSetWindowShouldClose(m_window, true);
}

bool hyp::Window::isRunning() const {
	return !glfwWindowShouldClose(m_window);
}

void hyp::Window::onUpdate() {
	glfwPollEvents();
	m_context->swapBuffer();
}

void hyp::Window::setEventCallback(const EventCallbackFn& fn) {
	m_props.windowData.event_callback = fn;
}

// ======= callback definition =======

void callback::registerWindowSize(GLFWwindow* window, int width, int height) {
	hyp::WindowData& data = *(hyp::WindowData*)glfwGetWindowUserPointer(window);

	data.width = width;
	data.height = height;

	hyp::WindowResizeEvent event(width, height);
	data.event_callback(event);
};

void callback::registerWindowClose(GLFWwindow* window) {
	hyp::WindowData& data = *(hyp::WindowData*)glfwGetWindowUserPointer(window);

	hyp::WindowCloseEvent event;
	data.event_callback(event);
}

void callback::registerKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	hyp::WindowData& data = *(hyp::WindowData*)glfwGetWindowUserPointer(window);

	switch (action)
	{
	case GLFW_PRESS:
	{
		hyp::KeyPressedEvent event((hyp::KeyCode)key, false);
		data.event_callback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		hyp::KeyReleasedEvent event((hyp::KeyCode)key);
		data.event_callback(event);
		break;
	}
	case GLFW_REPEAT:
	{
		hyp::KeyPressedEvent event((hyp::KeyCode)key, true);
		data.event_callback(event);
		break;
	}
	default:
		break;
	};
}
void callback::registerCursorPos(GLFWwindow* window, double xpos, double ypos) {
	hyp::WindowData& data = *(hyp::WindowData*)glfwGetWindowUserPointer(window);

	hyp::MouseMovedEvent event((float)xpos, (float)ypos);
	data.event_callback(event);
}

void callback::registerScroll(GLFWwindow* window, double xoffset, double yoffset) {
	hyp::WindowData& data = *(hyp::WindowData*)glfwGetWindowUserPointer(window);
	hyp::MouseScrolledEvent event((float)xoffset, (float)yoffset);
	data.event_callback(event);
}

void callback::registerMouseBtn(GLFWwindow* window, int button, int action, int mods) {
	hyp::WindowData& data = *(hyp::WindowData*)glfwGetWindowUserPointer(window);

	switch (action)
	{
	case GLFW_PRESS:
	{
		hyp::MouseBtnPressedEvent event(button);
		data.event_callback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		hyp::MouseBtnReleasedEvent event(button);
		data.event_callback(event);
		break;
	}
	default:
		break;
	}
}
