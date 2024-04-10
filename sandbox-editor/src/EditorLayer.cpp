#include "EditorLayer.hpp"
#include <imgui.h>
#include <renderer/renderer2d.hpp>

EditorLayer::EditorLayer()
	: Layer("editor-layer") {

	m_viewportSize = {600.f, 600.f};

	hyp::FramebufferSpecification fbSpec;
	fbSpec.attachment = hyp::FbAttachmentSpecification({{hyp::FbTextureFormat::RGBA}});
	fbSpec.width = 600;
	fbSpec.height = 600;
	m_framebuffer = hyp::Framebuffer::create(fbSpec);

	m_cameraController = hyp::CreateRef<hyp::OrthoGraphicCameraController>(600.f, 600.f);
}

void EditorLayer::onEvent(hyp::Event& event) {
	m_cameraController->onEvent(event);
}

void EditorLayer::onUpdate(float dt) {
	if (m_viewportFocused) {
		m_cameraController->onUpdate(dt);
	}

	m_framebuffer->bind();
	hyp::RenderCommand::setClearColor(0.3, 0.4, 0.1, 1.f);
	hyp::RenderCommand::clear();

	hyp::Renderer2D::beginScene(m_cameraController->getCamera().getViewProjectionMatrix());
	hyp::Renderer2D::drawQuad(glm::vec3(0.f), glm::vec2(300.f, 300.f), glm::vec4(1.0));
	hyp::Renderer2D::endScene();

	m_framebuffer->unbind();
}

void EditorLayer::onUIRender() {
	static bool demo = true;

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) hyp::Application::get().close();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

	ImGui::Begin("Viewport");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	m_viewportFocused = ImGui::IsWindowFocused();
	auto hovered = ImGui::IsWindowHovered();

	hyp::Application::get().getUILayer()->blockEvent(!hovered);

	if (m_viewportSize != *(( glm::vec2* )&viewportPanelSize)) {
		m_framebuffer->resize(( uint32_t )viewportPanelSize.x, ( uint32_t )viewportPanelSize.y);
		m_viewportSize = {viewportPanelSize.x, viewportPanelSize.y};

		m_cameraController->onResize(viewportPanelSize.x, viewportPanelSize.y);
	}

	uint32_t textureID = m_framebuffer->getColorAttachmentId();
	ImGui::Image(( void* )textureID, ImVec2{m_viewportSize.x, m_viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::ShowDemoWindow(&demo);
}
