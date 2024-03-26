#ifndef HYPER_APPLICATION_HPP
#define HYPER_APPLICATION_HPP

#include <glad/glad.h>
#include <core/window.hpp>
#include <core/layer_stack.hpp>

int main(int argc, char** argv);

namespace hyp {


	class HYPER_API Application : public hyp::NonCopyable {
	public:
		Application(const WindowProps& ws);

		static Application& get() {
			return *sInstance;
		}
		
		const hyp::Unique<Window>& getWindow() const;

		void onEvent(Event& e);

		void close();

		void pushLayer(Layer* layer);
	private:
		bool onResize(const WindowResizeEvent&);
		bool onWindowClose(const WindowCloseEvent&);

		void run();
	private:
		bool m_minimized = false;
		bool m_running = false;
		hyp::Scope<Window> m_window;
	private:
		friend int ::main(int, char**);
		static hyp::Application* sInstance;
		hyp::LayerStack m_layerStack;
	};
}

#endif