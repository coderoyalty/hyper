#ifndef HYPER_APPLICATION_HPP
#define HYPER_APPLICATION_HPP

#include <glad/glad.h>
#include <core/window.hpp>

namespace hyp {
	class HYPER_API Application : public hyp::NonCopyable {
	public:
		Application(const WindowProps& ws);
		void run();

		static Application& get() {
			return *sInstance;
		}
		
		GLFWwindow* getWindow() const;

	private:
		void close();

		void onEvent(Event& e);

		bool onResize(const WindowResizeEvent&);

	private:
		bool m_minimized = false;
		bool m_running = false;
		std::unique_ptr<Window> m_window;
	private:
		static hyp::Application* sInstance;

	};
}

#endif