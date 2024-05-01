#include "EditorLayer.hpp"
#include <renderer/renderer2d.hpp>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

#include <core/version.hpp>

using namespace hyp;
using namespace editor;

namespace utils {
	static bool decomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

	static bool showUserGuideWindow = false;

	static void editorUserGuide() {
		if (!utils::showUserGuideWindow)
		{
			return;
		}

		ImGui::Begin("Dev Guide", &showUserGuideWindow);

		ImGui::Text("Welcome to Hyper-Engine %d.%d.%d!", HYPER_VERSION_MAJOR, HYPER_VERSION_MINOR, HYPER_VERSION_PATCH);
		ImGui::Spacing();

		ImGui::Text("Controls:");
		ImGui::BulletText("LEFT_ALT + Mouse: For Editor Camera Movement");
		ImGui::BulletText("Q: For Disabling Gizmo operation");
		ImGui::BulletText("G: To Move the selected entity");
		ImGui::BulletText("R: To Rotate the selected entity");
		ImGui::BulletText("S: To scale the selected entity");
		ImGui::Spacing();

		ImGui::Text("Simple Guide");

		ImGui::TextWrapped("%s %s\n\n%s %s\n%s",
		    "Firstly, in the scene hierarchy panel, right-click and select the popup menu to create an entity.",
		    "Select the new entity (already done by default), you should see its components rendered in the properties panel.",
		    "Nothing is rendered on the viewport for now, that's because the current entity does not have any drawable component yet.",
		    "You should see a button labeled `add component`, click it and select your prefered drawable component (for the sake of this doc, choose SpriteComponent).",
		    "Congrats! You can play around with the entity, and also try using the controls (see above) to manipulate your entity");

		ImGui::Spacing();

		if (ImGui::Button("Close"))
		{
			showUserGuideWindow = false;
		}

		ImGui::End();
	}
}

EditorLayer::EditorLayer()
    : Layer("editor-layer") {
	m_viewportInfo.size = { 600.f, 600.f };
	m_viewportInfo.min_bound = glm::vec2(0.0);
	m_viewportInfo.max_bound = glm::vec2(0.0);

	hyp::FramebufferSpecification fbSpec;
	fbSpec.attachment = hyp::FbAttachmentSpecification({ { hyp::FbTextureFormat::RGBA }, { hyp::FbTextureFormat::RED_INT } });
	fbSpec.width = 600;
	fbSpec.height = 600;
	m_framebuffer = hyp::Framebuffer::create(fbSpec);

	m_scene = hyp::CreateRef<hyp::Scene>();

	m_hierarchyPanel = hyp::CreateRef<hyp::HierarchyPanel>(m_scene);

	m_editorCamera.setPosition(glm::vec3(0.f, 0.f, 3.f));

	auto& entity = m_scene->createEntity("Square");
	auto& sc = entity.getOrAdd<hyp::SpriteRendererComponent>();
	sc.texture = hyp::Texture2D::create("assets/textures/checkerboard.png");
}

void EditorLayer::onEvent(hyp::Event& event) {
	m_editorCamera.onEvent(event);
	hyp::EventDispatcher ed(event);
	ed.dispatch<hyp::KeyPressedEvent>(BIND_EVENT_FN(onKeyPressed));
	ed.dispatch<hyp::MouseBtnPressedEvent>(BIND_EVENT_FN(onMousePressed));
}

void EditorLayer::onUpdate(float dt) {
	if (m_viewportInfo.focused)
	{
		m_editorCamera.onUpdate(dt);
	}

	m_framebuffer->bind();
	hyp::RenderCommand::setClearColor(0.1, 0.1, 0.1, 1.f);
	hyp::RenderCommand::clear();

	hyp::Renderer2D::beginScene(m_editorCamera.getViewProjectionMatrix());
	m_scene->onUpdate(dt);
	hyp::Renderer2D::endScene();

	m_framebuffer->clearAttachment(1, -1);
	auto [mx, my] = ImGui::GetMousePos();

	mx -= m_viewportInfo.bounds[0].x;
	my -= m_viewportInfo.bounds[0].y;
	glm::vec2 viewportSize = m_viewportInfo.bounds[1] - m_viewportInfo.bounds[0];
	my = viewportSize.y - my;
	int mouseX = (int)mx;
	int mouseY = (int)my;

	if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
	{
		int pixel = m_framebuffer->readPixel(1, mouseX, mouseY);
		// Note: buggy assumption... (10k maximum entities)
		m_hoveredEntity = pixel > 10000 ? hyp::Entity() : hyp::Entity((entt::entity)pixel, m_scene.get());
	}

	m_framebuffer->unbind();
}

void EditorLayer::onUIRender() {
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				;

			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				;

			if (ImGui::MenuItem("Save Scene As", "Ctrl+Shift+S"))
				;

			ImGui::Separator();

			if (ImGui::MenuItem("Exit")) hyp::Application::get().close();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Dev Guide"))
			{
				utils::showUserGuideWindow = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	static bool show_demo = true;

	ImGui::ShowDemoWindow(&show_demo);

	utils::editorUserGuide();

	m_hierarchyPanel->onUIRender();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 { 0, 0 });

	ImGui::Begin("Viewport");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	m_viewportInfo.focused = ImGui::IsWindowFocused();
	m_viewportInfo.hovered = ImGui::IsWindowHovered();

	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();

	m_viewportInfo.bounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_viewportInfo.bounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	hyp::Application::get().getUILayer()->blockEvent(!m_viewportInfo.hovered);

	if (m_viewportInfo.size != *((glm::vec2*)&viewportPanelSize))
	{
		m_framebuffer->resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		m_viewportInfo.size = { viewportPanelSize.x, viewportPanelSize.y };

		m_editorCamera.setViewport(viewportPanelSize.x, viewportPanelSize.y);
	}

	uint32_t textureID = m_framebuffer->getColorAttachmentId(0);
	ImGui::Image((void*)textureID, ImVec2 { m_viewportInfo.size.x, m_viewportInfo.size.y },
	    ImVec2 { 0, 1 }, ImVec2 { 1, 0 });

	auto selectedEntity = m_hierarchyPanel->getSelectedEntity();

	if (selectedEntity && selectedEntity.has<TransformComponent>() && m_gizmoType != -1)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(m_viewportInfo.bounds[0].x, m_viewportInfo.bounds[0].y,
		    m_viewportInfo.bounds[1].x - m_viewportInfo.bounds[0].x, m_viewportInfo.bounds[1].y - m_viewportInfo.bounds[0].y);
		auto& camera = m_editorCamera;

		const glm::mat4& cameraProjection = camera.getProjectionMatrix();
		glm::mat4 cameraView = camera.getViewMatrix();
		auto& tc = selectedEntity.get<hyp::TransformComponent>();
		glm::mat4 transform = tc.getTransform();

		ImGuizmo::Manipulate(
		    glm::value_ptr(cameraView),
		    glm::value_ptr(cameraProjection),
		    (ImGuizmo::OPERATION)m_gizmoType,
		    ImGuizmo::LOCAL,
		    glm::value_ptr(transform));

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 translation, rotation, scale;
			utils::decomposeTransform(transform, translation, rotation, scale);

			glm::vec3 deltaRotation = rotation - tc.rotation;
			tc.position = translation;
			tc.rotation += deltaRotation;
			tc.scale = scale;
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

bool EditorLayer::onMousePressed(hyp::MouseBtnPressedEvent& event) {
	if (event.getButton() == hyp::Mouse::BUTTON_LEFT)
	{
		if (m_viewportInfo.hovered &&
		    !ImGuizmo::IsOver() &&
		    !hyp::Input::isKeyPressed(hyp::LEFT_ALT))
		{
			m_hierarchyPanel->setSelectedEntity(m_hoveredEntity);
		}
	}

	return false;
}

bool hyp::editor::EditorLayer::onKeyPressed(hyp::KeyPressedEvent& event) {
	if (event.isRepeat())
		return false;

	switch (event.getkey())
	{
	case hyp::Key::Q:
	{
		if (!ImGuizmo::IsUsing())
		{
			m_gizmoType = -1;
		};
		break;
	}
	case hyp::Key::S: // scale
	{
		if (!ImGuizmo::IsUsing())
		{
			m_gizmoType = ImGuizmo::SCALE;
		}
		break;
	}
	case hyp::Key::R: // Rotate
	{
		if (!ImGuizmo::IsUsing())
		{
			m_gizmoType = ImGuizmo::ROTATE;
		}
		break;
	}
	case hyp::Key::G: // translate
	{
		if (!ImGuizmo::IsUsing())
		{
			m_gizmoType = ImGuizmo::TRANSLATE;
		}
		break;
	}
	default:
		break;
	}

	return false;
}

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

bool utils::decomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) {
	// From glm::decompose in matrix_decompose.inl

	using namespace glm;
	using T = float;

	mat4 LocalMatrix(transform);

	// Normalize the matrix.
	if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
		return false;

	// First, isolate perspective.  This is the messiest.
	if (
	    epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
	    epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
	    epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
	{
		// Clear the perspective partition
		LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
		LocalMatrix[3][3] = static_cast<T>(1);
	}

	// Next take care of translation (easy).
	translation = vec3(LocalMatrix[3]);
	LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

	vec3 Row[3] {};

	// Now get scale and shear.
	for (length_t i = 0; i < 3; ++i)
		for (length_t j = 0; j < 3; ++j)
			Row[i][j] = LocalMatrix[i][j];

	// Compute X scale factor and normalize first row.
	scale.x = length(Row[0]);
	Row[0] = detail::scale(Row[0], static_cast<T>(1));
	scale.y = length(Row[1]);
	Row[1] = detail::scale(Row[1], static_cast<T>(1));
	scale.z = length(Row[2]);
	Row[2] = detail::scale(Row[2], static_cast<T>(1));

	rotation.y = asin(-Row[0][2]);
	if (cos(rotation.y) != 0)
	{
		rotation.x = atan2(Row[1][2], Row[2][2]);
		rotation.z = atan2(Row[0][1], Row[0][0]);
	}
	else
	{
		rotation.x = atan2(-Row[2][0], Row[1][1]);
		rotation.z = 0;
	}

	return true;
}
