#include <ui/imgui_layer.hpp>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/application.hpp>

void hyp::ImGuiLayer::onAttach() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	GLFWwindow* window = hyp::Application::get().getWindow()->getNativeWindow();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void hyp::ImGuiLayer::onDetach() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void hyp::ImGuiLayer::onEvent(hyp::Event& e) {
	if (m_blockEvents)
	{
		auto& io = ImGui::GetIO();
		e.handled |= e.belongsToCategory(hyp::EventCategory::MouseCategory) & io.WantCaptureMouse;
		e.handled |= e.belongsToCategory(hyp::EventCategory::KeyboardCategory) & io.WantCaptureKeyboard;
	}
}

void hyp::ImGuiLayer::begin() {
	auto& io = ImGui::GetIO();
	auto& app = hyp::Application::get();
	io.DisplaySize = ImVec2((float)app.getWindow()->getWidth(), (float)app.getWindow()->getHeight());

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void hyp::ImGuiLayer::end() {
	auto& io = ImGui::GetIO();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void hyp::ImGuiLayer::blockEvent(bool value) {
	m_blockEvents = value;
}
