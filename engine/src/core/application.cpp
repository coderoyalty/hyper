#include <core/application.hpp>
#include <utils/logger.hpp>

hyp::Application *hyp::Application::sInstance = nullptr;

hyp::Application::Application(const WindowProps& ws)
{
	sInstance = this;

	m_window = hyp::Window::create(ws);
	m_running = true;
	m_window->setEventCallback(BIND_EVENT_FN(Application::onEvent));
}

void hyp::Application::run()
{
	while (m_running && m_window->isRunning()) {
		m_window->onUpdate();
	}
}

GLFWwindow* hyp::Application::getWindow() const
{
	return m_window->getNativeWindow();
}

void hyp::Application::close()
{
	m_window->close();
	m_running = false;
}

void hyp::Application::onEvent(Event& e)
{
	hyp::EventDispatcher ed(e);
	ed.dispatch<hyp::WindowResizeEvent>(BIND_EVENT_FN(Application::onResize));
}

bool hyp::Application::onResize(const WindowResizeEvent& event)
{
	if (event.getWidth() == 0 || event.getHeight() == 0)
	{
		m_minimized = true;
		return false;
	}
	m_minimized = false;
	return true;
}


