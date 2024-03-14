#include "core/window.hpp"
#include <utils/logger.hpp>
#include "window.hpp"

hyp::Scope<hyp::Window> hyp::Window::createWindow(WindowProps props)
{
	return hyp::CreateScope<hyp::Window>(props);
}

hyp::Window::Window(WindowProps props) : m_props(props)
{
	this->init();
}

hyp::Window::~Window()
{
	this->close();
	this->deinit();
}
void hyp::Window::init()
{
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

	m_context = hyp::CreateScope<hyp::OpenglContext>(m_window);
	m_context->init();

	if (m_props.maximized)
	{
		int w, h;
		glfwGetWindowSize(m_window, &w, &h);
		data.width = w;
		data.height = h;
	}
}

void hyp::Window::deinit()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);
	}
	return;
}

void hyp::Window::setVisibility(bool visible)
{
	glfwSetWindowAttrib(m_window, GLFW_VISIBLE, visible);
	m_props.visible = visible;
}

void hyp::Window::close()
{
	glfwSetWindowShouldClose(m_window, true);
}

bool hyp::Window::isRunning() const
{
	return !glfwWindowShouldClose(m_window);
}

void hyp::Window::onUpdate()
{
	glfwPollEvents();
	m_context->swapBuffer();
}
