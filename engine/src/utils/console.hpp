#pragma once
#ifndef HYP_CONSOLE_HPP
	#define HYP_CONSOLE_HPP

	#include <imgui.h>
	#include <string>
	#include <vector>

namespace hyp {
	class Console {
	public:
		Console();

		void clear();
		void log(const char* fmt, ...) IM_FMTARGS(2);
		void render(const std::string& str, bool* p_open);

		static Console& get();

	private:
		ImGuiTextBuffer m_buffer;
		std::vector<uint32_t> m_lineOffsets;

		bool m_scrollToBottom;
	};

	#define CONSOLE_LOG(fmt, ...) hyp::Console::get().log(fmt, ##__VA_ARGS__);
}

#endif