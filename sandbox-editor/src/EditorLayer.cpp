#include "EditorLayer.hpp"
#include <renderer/renderer2d.hpp>
#include <glm/gtc/type_ptr.hpp>

EditorLayer::EditorLayer()
    : Layer("editor-layer") {
	m_viewportSize = { 600.f, 600.f };

	hyp::FramebufferSpecification fbSpec;
	fbSpec.attachment = hyp::FbAttachmentSpecification({ { hyp::FbTextureFormat::RGBA }, { hyp::FbTextureFormat::RED_INT } });
	fbSpec.width = 600;
	fbSpec.height = 600;
	m_framebuffer = hyp::Framebuffer::create(fbSpec);

	m_cameraController = hyp::CreateRef<hyp::OrthoGraphicCameraController>(600.f, 600.f);

	m_scene = hyp::CreateRef<hyp::Scene>();

	m_hierarchyPanel = hyp::CreateRef<hyp::HierarchyPanel>(m_scene);

	m_entity = m_scene->createEntity("Square");

	m_entity.get<hyp::TransformComponent>().size = { 100.f, 100.f };
	m_entity.add<hyp::SpriteRendererComponent>(glm::vec4(1.0));
}

void EditorLayer::onEvent(hyp::Event& event) {
	m_cameraController->onEvent(event);
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

	/*m_framebuffer->clearAttachment(1, -1);
	auto [mx, my] = ImGui::GetMousePos();

	mx -= m_viewportBounds[0].x;
	my -= m_viewportBounds[0].y;
	glm::vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
	my = viewportSize.y - my;
	int mouseX = (int)mx;
	int mouseY = (int)my;*/

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
