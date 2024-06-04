#include "pch.h"
#include "console.hpp"

static hyp::Console s_console;

hyp::Console::Console() {
	clear();
	log("Hyper-Editor Console\n");
}

void hyp::Console::clear() {
	m_buffer.clear();
	m_lineOffsets.clear();
	m_lineOffsets.push_back(0);
}

void hyp::Console::log(const char* fmt, ...) IM_FMTARGS(2) {
	int old_size = m_buffer.size();

	va_list args;
	va_start(args, fmt);
	m_buffer.appendfv(fmt, args);
	va_end(args);

	m_buffer.append("\n");

	for (int new_size = m_buffer.size(); old_size < new_size; old_size++)
		if (m_buffer[old_size] == '\n')
			m_lineOffsets.push_back(old_size + 1);
}

void hyp::Console::render(const std::string& title, bool* p_open) {
	static char InputBuf[256];

	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(title.c_str(), p_open))
	{
		ImGui::End();
		return;
	}

	if (ImGui::Button("Clear")) clear();

	ImGui::SameLine();
	bool copy_to_clipboard = ImGui::Button("Copy");

	ImGui::Separator();

	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Clear")) clear();
		ImGui::EndPopup();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
	if (copy_to_clipboard)
		ImGui::LogToClipboard();

	ImGui::TextUnformatted(m_buffer.begin());

	if (copy_to_clipboard)
		ImGui::LogFinish();

	if (m_scrollToBottom)
		ImGui::SetScrollHereY(1.0f);
	m_scrollToBottom = false;

	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();

	bool reclaim_focus = false;
	if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf)))
	{
		char* s = InputBuf;
		strcpy(s, "");
		reclaim_focus = true;
	}

	ImGui::SetItemDefaultFocus();
	if (reclaim_focus)
		ImGui::SetKeyboardFocusHere(-1);

	ImGui::End();
}

hyp::Console& hyp::Console::get() {
	// TODO: insert return statement here
	return s_console;
}
