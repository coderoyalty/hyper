#pragma once

#ifndef HYPER_WINDOW_HPP
#define HYPER_WINDOW_HPP

#include <string>
#include <GLFW/glfw3.h>
#include <system/export.hpp>
#include <core/base.hpp>
#include <opengl/context.hpp>
#include <utils/non_copyable.hpp>
#include <event/event.hpp>
#include <event/win_event.hpp>

namespace hyp
{

	using EventCallbackFn = std::function<void(Event&)>;
	struct HYPER_API WindowData
	{
		int width;
		int height;
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
		WindowData windowData;

		WindowProps(const std::string& title = "hyper-engine", int width = 1080, int height = 720)
		{
			this->windowData = { width, height, title };
		}
	};

	class HYPER_API Window : public hyp::NonCopyable
	{
	public:
		Window(WindowProps props);
		~Window();
		static hyp::Scope<Window> create(WindowProps props = WindowProps());
		GLFWwindow* getRawWindow() const
		{
			return this->m_window;
		};

		bool isRunning() const;

		void onUpdate();

		void setEventCallback(const EventCallbackFn& fn);

	private:
		void init();
		void deinit();

		void close();
		void setVisibility(bool visible);

	private:
		hyp::Scope<OpenglContext> m_context;
		WindowProps m_props;
		GLFWwindow* m_window;

		friend class Application;
	};
}

#endif