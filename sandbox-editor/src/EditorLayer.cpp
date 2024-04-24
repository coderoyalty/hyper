#include "EditorLayer.hpp"
#include <renderer/renderer2d.hpp>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

EditorLayer::EditorLayer()
    : Layer("editor-layer") {
	m_viewportSize = { 600.f, 600.f };
	m_viewportBounds[0] = glm::vec2(0.0);
	m_viewportBounds[1] = glm::vec2(0.0);

	hyp::FramebufferSpecification fbSpec;
	fbSpec.attachment = hyp::FbAttachmentSpecification({ { hyp::FbTextureFormat::RGBA }, { hyp::FbTextureFormat::RED_INT } });
	fbSpec.width = 600;
	fbSpec.height = 600;
	m_framebuffer = hyp::Framebuffer::create(fbSpec);

	m_cameraController = hyp::CreateRef<hyp::OrthoGraphicCameraController>(600.f, 600.f);

	m_scene = hyp::CreateRef<hyp::Scene>();

	m_hierarchyPanel = hyp::CreateRef<hyp::HierarchyPanel>(m_scene);
}

void EditorLayer::onEvent(hyp::Event& event) {
	m_cameraController->onEvent(event);

	hyp::EventDispatcher ed(event);
	ed.dispatch<hyp::MouseBtnPressedEvent>(BIND_EVENT_FN(onMousePressed));
}

void EditorLayer::onUpdate(float dt) {
	if (m_viewportFocused)
	{
		m_cameraController->onUpdate(dt);
	}

	m_framebuffer->bind();
	hyp::RenderCommand::setClearColor(0.3, 0.4, 0.1, 1.f);
	hyp::RenderCommand::clear();

	hyp::Renderer2D::beginScene(m_cameraController->getCamera().getViewProjectionMatrix());
	m_scene->onUpdate(dt);
	hyp::Renderer2D::endScene();

	m_framebuffer->clearAttachment(1, -1);
	auto [mx, my] = ImGui::GetMousePos();

	mx -= m_viewportBounds[0].x;
	my -= m_viewportBounds[0].y;
	glm::vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
	my = viewportSize.y - my;
	int mouseX = (int)mx;
	int mouseY = (int)my;

	if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
	{
		int pixel = m_framebuffer->readPixel(1, mouseX, mouseY);
		m_hoveredEntity = pixel == 1050253722 ? hyp::Entity() : hyp::Entity((entt::entity)pixel, m_scene.get());
	}

	m_framebuffer->unbind();
}

void EditorLayer::onUIRender() {
	static bool demo = true;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) hyp::Application::get().close();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 { 0, 0 });

	ImGui::Begin("Viewport");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	m_viewportFocused = ImGui::IsWindowFocused();
	auto hovered = ImGui::IsWindowHovered();

	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();

	m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	hyp::Application::get().getUILayer()->blockEvent(!hovered);

	if (m_viewportSize != *((glm::vec2*)&viewportPanelSize))
	{
		m_framebuffer->resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		m_cameraController->onResize(viewportPanelSize.x, viewportPanelSize.y);
	}

	uint32_t textureID = m_framebuffer->getColorAttachmentId(0);
	ImGui::Image((void*)textureID, ImVec2 { m_viewportSize.x, m_viewportSize.y }, ImVec2 { 0, 1 }, ImVec2 { 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::ShowDemoWindow(&demo);

	m_hierarchyPanel->onUIRender();
}

bool EditorLayer::onMousePressed(hyp::MouseBtnPressedEvent& event) {
	if (event.getButton() == hyp::Mouse::BUTTON_LEFT)
	{
		m_hierarchyPanel->setSelectedEntity(m_hoveredEntity);
	}

	return false;
}
