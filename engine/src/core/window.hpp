#pragma once

#ifndef HYPER_WINDOW_HPP
	#define HYPER_WINDOW_HPP

	#include <GLFW/glfw3.h>
	#include <core/base.hpp>
	#include <event/event.hpp>
	#include <event/key_event.hpp>
	#include <event/mouse_event.hpp>
	#include <event/win_event.hpp>
	#include <opengl/context.hpp>
	#include <string>
	#include <system/export.hpp>
	#include <utils/non_copyable.hpp>

namespace hyp {

	using EventCallbackFn = std::function<void(Event&)>;
	struct HYPER_API WindowData
	{
		int width = 0;
		int height = 0;
		std::string title;
		EventCallbackFn event_callback;
	};

	struct HYPER_API WindowProps
	{
		bool maximized = false;
		bool fullscreen = false;
		bool resizable = true;
		bool visible = true;
		bool focus = true;
		bool vsync = false;

		WindowData windowData;

		WindowProps(const std::string& title = "hyper-engine", int width = 1080, int height = 720) { this->windowData = { width, height, title }; }
	};

	class HYPER_API Window : public hyp::NonCopyable {
	public:
		Window(WindowProps props);
		~Window();
		static hyp::Scope<Window> create(WindowProps props = WindowProps());

		GLFWwindow* getNativeWindow() const { return this->m_window; };

		bool isRunning() const;

		void onUpdate();

		void setEventCallback(const EventCallbackFn& fn);

		uint32_t getWidth() const { return this->m_props.windowData.width; }

		uint32_t getHeight() const { return this->m_props.windowData.height; }

		void setVSync(bool active);

		bool isVSync() const { return m_props.vsync; }

		void setVisibility(bool visible);

	private:
		void init();
		void deinit();

		void close();

	private:
		hyp::Scope<OpenglContext> m_context;
		WindowProps m_props;
		GLFWwindow* m_window;

		friend class Application;
	};
} // namespace hyp

#endif