#include <core/application.hpp>
#include <core/timer.hpp>
#include <utils/logger.hpp>

hyp::Application* hyp::Application::sInstance = nullptr;

hyp::Application::Application(const WindowProps& ws) {
	HYP_ASSERT_CORE(sInstance == nullptr, "application already exists");
	sInstance = this;

	m_window = hyp::Window::create(ws);
	m_running = true;
	m_window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

	this->m_uiLayer = new hyp::ImGuiLayer();

	pushOverlay(this->m_uiLayer);
}

void hyp::Application::run() {
	float last_frame = 0.f;
	while (m_running && m_window->isRunning())
	{
		hyp::Timer::postTick();

		float dt = hyp::Timer::getDeltaTime();

		if (!this->m_minimized)
		{
			for (auto layer : m_layerStack)
			{
				layer->onUpdate(dt);
			}
		}

		m_uiLayer->begin();

		{
			for (auto layer : m_layerStack)
			{
				layer->onUIRender();
			}
		}

		m_uiLayer->end();

		m_window->onUpdate();
	}
}

const hyp::Unique<hyp::Window>& hyp::Application::getWindow() const {
	return m_window;
}

void hyp::Application::close() {
	m_window->close();
	m_running = false;
}

void hyp::Application::pushLayer(Layer* layer) {
	m_layerStack.pushLayer(layer);
	layer->onAttach();
}

void hyp::Application::pushOverlay(Layer* overlay) {
	m_layerStack.pushOverlay(overlay);
	overlay->onAttach();
}

void hyp::Application::onEvent(Event& e) {
	hyp::EventDispatcher ed(e);
	ed.dispatch<hyp::WindowResizeEvent>(BIND_EVENT_FN(Application::onResize));
	ed.dispatch<hyp::WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

	for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it++)
	{
		if (e.handled)
		{
			break;
		}

		(*it)->onEvent(e);
	}
}

bool hyp::Application::onResize(const WindowResizeEvent& event) {
	if (event.getWidth() == 0 || event.getHeight() == 0)
	{
		m_minimized = true;
		return false;
	}
	m_minimized = false;
	return false;
}

bool hyp::Application::onWindowClose(const WindowCloseEvent&) {
	m_running = false;
	return false;
}
