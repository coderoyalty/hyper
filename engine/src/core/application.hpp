#ifndef HYPER_APPLICATION_HPP
#define HYPER_APPLICATION_HPP
// clang-format off
#include <glad/glad.h>
#include <core/layer_stack.hpp>
#include <core/window.hpp>
#include <ui/imgui_layer.hpp>
// clang-format on

int main(int argc, char** argv);

namespace hyp {

	class HYPER_API Application : public hyp::NonCopyable {
	public:
		Application(const WindowProps& ws);

		static Application& get() { return *sInstance; }

		const hyp::Unique<Window>& getWindow() const;

		void onEvent(Event& e);

		void close();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		hyp::ImGuiLayer* getUILayer() { return m_uiLayer; }

	private:
		bool onResize(const WindowResizeEvent&);
		bool onWindowClose(const WindowCloseEvent&);
		void run();

	private:
		bool m_minimized = false;
		bool m_running = false;
		hyp::Scope<Window> m_window;
		hyp::ImGuiLayer* m_uiLayer;
		hyp::LayerStack m_layerStack;

	private:
		friend int ::main(int, char**);
		static hyp::Application* sInstance;
	};
} // namespace hyp

#endif