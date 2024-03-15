#include <core/application.hpp>

hyp::Application *hyp::Application::sInstance = nullptr;

hyp::Application::Application(const WindowProps& ws)
{
	sInstance = this;

	m_window = hyp::Window::create(ws);
	m_running = true;
}

void hyp::Application::run()
{
	while (m_running && m_window->isRunning()) {
		m_window->onUpdate();
	}
}

GLFWwindow* hyp::Application::getGLFWwindow() const
{
	return m_window->getRawWindow();
}

void hyp::Application::close()
{
	m_window->close();
	m_running = false;
}


