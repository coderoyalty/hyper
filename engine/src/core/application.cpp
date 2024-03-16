#include <core/application.hpp>
#include <utils/logger.hpp>

hyp::Application* hyp::Application::sInstance = nullptr;

hyp::Application::Application(const WindowProps& ws)
{
	HYP_ASSERT_CORE(sInstance == nullptr, "application already exists");
	sInstance = this;

	m_window = hyp::Window::create(ws);
	m_running = true;
	m_window->setEventCallback(BIND_EVENT_FN(Application::onEvent));
}

void hyp::Application::run()
{
	float m_lastTime = 0.f;
	while (m_running && m_window->isRunning()) {

		//TODO: get delta-time
		float dt = m_lastTime;


		if (!this->m_minimized) {
			for (auto layer : m_layerStack) {
				layer->onUpdate(dt);
			}
		}
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

void hyp::Application::pushLayer(Layer* layer)
{
	m_layerStack.pushLayer(layer);
	layer->onAttach();
}

void hyp::Application::onEvent(Event& e)
{
	hyp::EventDispatcher ed(e);
	ed.dispatch<hyp::WindowResizeEvent>(BIND_EVENT_FN(Application::onResize));
	ed.dispatch<hyp::WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

	for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it++) {
		if (e.handled) {
			break;
		}

		(*it)->onEvent(e);
	}
}

bool hyp::Application::onResize(const WindowResizeEvent& event)
{
	if (event.getWidth() == 0 || event.getHeight() == 0)
	{
		m_minimized = true;
		return false;
	}
	m_minimized = false;
	return false;
}

bool hyp::Application::onWindowClose(const WindowCloseEvent&)
{
	m_running = false;
	return false;
}


