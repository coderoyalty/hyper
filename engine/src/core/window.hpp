#pragma once

#ifndef HYPER_WINDOW_HPP
#define HYPER_WINDOW_HPP

#include <string>
#include <GLFW/glfw3.h>
#include <system/export.hpp>
#include <core/base.hpp>
#include <opengl/context.hpp>

namespace hyp
{
	struct HYPER_API WindowData
	{
		int width;
		int height;
		std::string title;
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

	class HYPER_API Window
	{
	public:
		Window(WindowProps props);
		~Window();
		static hyp::Scope<Window> createWindow(WindowProps props);
		GLFWwindow* getRawWindow() const
		{
			return this->m_window;
		};

		bool isRunning() const;

		void onUpdate();

	private:
		void init();
		void deinit();

		void close();
		void setVisibility(bool visible);

	private:
		hyp::Scope<OpenglContext> m_context;
		WindowProps m_props;
		GLFWwindow* m_window;
	};
}

#endif