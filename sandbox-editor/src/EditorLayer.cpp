#include "EditorLayer.hpp"
#include <renderer/renderer2d.hpp>
#include <ImGuizmo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/version.hpp>
#include <scene/serializer.hpp>
#include <utils/file_dialog.hpp>
#include <scripting/script_engine.hpp>

using namespace hyp;
using namespace editor;

namespace utils {
	static bool decomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

	static void editorUserGuide(bool& show) {
		if (!show)
		{
			return;
		}

		ImGui::Begin("Dev Guide", &show);

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
			show = false;
		}

		ImGui::End();
	}
}

EditorLayer::EditorLayer()
    : Layer("editor-layer"), m_cameraController(600.f, 600.f) {
	m_gridProgram = hyp::ShaderProgram::create("assets/shaders/grid.vert", "assets/shaders/grid.frag");
	m_gridProgram->link();

	float vertices[] = {
		// Vertex positions
		1.0, 1.0, 0.0,
		-1.0, -1.0, 0.0,
		-1.0, 1.0, 0.0,
		-1.0, -1.0, 0.0,
		1.0, 1.0, 0.0,
		1.0, -1.0, 0.0
	};

	m_gridVao = hyp::VertexArray::create();
	auto& m_gridVbo = hyp::VertexBuffer::create(vertices, sizeof(vertices));

	m_gridVbo->setLayout({
	    hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec3, "aPos", false),
	});

	m_gridVao->addVertexBuffer(m_gridVbo);
}

void hyp::editor::EditorLayer::onAttach() {
	m_viewportInfo.size = { 600.f, 600.f };
	m_viewportInfo.min_bound = glm::vec2(0.0);
	m_viewportInfo.max_bound = glm::vec2(0.0);

	hyp::FramebufferSpecification fbSpec;
	fbSpec.attachment = hyp::FbAttachmentSpecification({ { hyp::FbTextureFormat::RGBA }, { hyp::FbTextureFormat::RED_INT },
	    { FbTextureFormat::DEPTH24_STENCIL8 } });
	fbSpec.width = 600;
	fbSpec.height = 600;
	m_framebuffer = hyp::Framebuffer::create(fbSpec);

	m_iconPlay = hyp::Texture2D::create("resources/icons/play.png");
	m_iconStop = hyp::Texture2D::create("resources/icons/stop.png");
	m_iconPause = hyp::Texture2D::create("resources/icons/pause.png");

	m_iconPlay->setFilter(hyp::TextureParam::NEAREST);
	m_iconPause->setFilter(hyp::TextureParam::NEAREST);
	m_iconStop->setFilter(hyp::TextureParam::NEAREST);

	m_editorScene = hyp::CreateRef<hyp::Scene>();
	m_activeScene = m_editorScene;

	m_hierarchyPanel = hyp::CreateRef<hyp::HierarchyPanel>(m_activeScene);

	m_editorCamera = hyp::EditorCamera(30.f, 1.778f, 0.1, 1000.f);
}

void hyp::editor::EditorLayer::onDetach() {
}

void EditorLayer::onEvent(hyp::Event& event) {
	m_editorCamera.onEvent(event);
	m_cameraController.onEvent(event);
	hyp::EventDispatcher ed(event);
	ed.dispatch<hyp::KeyPressedEvent>(BIND_EVENT_FN(onKeyPressed));
	ed.dispatch<hyp::MouseBtnPressedEvent>(BIND_EVENT_FN(onMousePressed));
}

void EditorLayer::onUpdate(float dt) {
	if (auto spec = m_framebuffer->getSpecification();
	    m_viewportInfo.size.x > 0.0f && m_viewportInfo.size.y > 0.0f &&
	    (spec.width != m_viewportInfo.size.x || spec.height != m_viewportInfo.size.y))
	{
		m_framebuffer->resize((uint32_t)m_viewportInfo.size.x, (uint32_t)m_viewportInfo.size.y);
		m_editorCamera.setViewport(m_viewportInfo.size.x, m_viewportInfo.size.y);
		m_cameraController.onResize(m_viewportInfo.size.x, m_viewportInfo.size.y);
	}

	m_framebuffer->bind();
	hyp::RenderCommand::setClearColor(0.1, 0.1, 0.1, 1.f);
	hyp::RenderCommand::clear();

	m_framebuffer->clearAttachment(1, -1);

	switch (m_sceneState)
	{
	case hyp::editor::EditorLayer::SceneState::Play:
	{
		m_activeScene->onUpdateRuntime(dt);
		break;
	}
	case hyp::editor::EditorLayer::SceneState::Edit:
	{
		if (m_viewportInfo.focused)
		{
			m_cameraController.onUpdate(dt);
		}
		m_editorCamera.onUpdate(dt);
		break;
	}
	default:
		break;
	}

	// render grid
	if (m_settings.showGrid)
	{
		m_gridVao->bind();
		m_gridProgram->use();
		m_gridProgram->setMat4("viewProj", m_editorCamera.getViewProjectionMatrix());
		glDrawArrays(GL_TRIANGLES, 0, 6); // TODO: avoid using explicit opengl call...
	}

	switch (m_settings.cameraType)
	{
	case CameraType::Perspective:
	{
		hyp::Renderer2D::beginScene(m_editorCamera.getViewProjectionMatrix());
		break;
	}
	case CameraType::Orthographic:
	{
		hyp::Renderer2D::beginScene(m_cameraController.getCamera().getViewProjectionMatrix());
		break;
	}
	default:
		break;
	}
	m_activeScene->onUpdate(dt);

	hyp::Renderer2D::endScene();

	auto [mx, my] = ImGui::GetMousePos();

	//--> mouse selection logic...
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
		m_hoveredEntity = pixel > 10000 ? hyp::Entity() : hyp::Entity((entt::entity)pixel, m_activeScene.get());
	}
	//<-- mouse selection logic...

	m_framebuffer->unbind();
}

void EditorLayer::onUIRender() {
	//-- application main menu
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
				openScene();

			if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				newScene();

			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				saveScene();

			if (ImGui::MenuItem("Save Scene As", "Ctrl+Shift+S"))
				saveSceneAs();

			ImGui::Separator();

			if (ImGui::MenuItem("Exit")) hyp::Application::get().close();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Editor Grid", "Ctrl+Shift+G", &m_settings.showGrid))
			{
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Dev Guide", "F2", &m_settings.showHelpGuide))
			{
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	//-- camera choice
	ImGui::Begin("Camera Properties");
	if (ImGui::RadioButton("Orthographic", m_settings.cameraType == CameraType::Orthographic))
	{
		m_settings.cameraType = CameraType::Orthographic;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Perspective", m_settings.cameraType == CameraType::Perspective))
	{
		m_settings.cameraType = CameraType::Perspective;
	}
	ImGui::End();

	//-- editor user guide
	utils::editorUserGuide(m_settings.showHelpGuide);
	//-- render hierarchy panel
	m_hierarchyPanel->onUIRender();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 { 0, 0 });

	ImGui::SetWindowSize(ImVec2 { m_viewportInfo.size.x, m_viewportInfo.size.y });
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
		m_cameraController.onResize(viewportPanelSize.x, viewportPanelSize.y);
		m_editorCamera.setViewport(viewportPanelSize.x, viewportPanelSize.y);
	}

	uint32_t textureID = m_framebuffer->getColorAttachmentId(0);
	ImGui::Image((void*)textureID, ImVec2 { m_viewportInfo.size.x, m_viewportInfo.size.y },
	    ImVec2 { 0, 1 }, ImVec2 { 1, 0 });

	//----> ImGuizmo

	glm::mat4 cameraProjection;
	glm::mat4 cameraView;

	if (m_settings.cameraType == CameraType::Perspective)
	{
		cameraProjection = m_editorCamera.getProjectionMatrix();
		cameraView = m_editorCamera.getViewMatrix();
	}
	else
	{
		cameraProjection = m_cameraController.getCamera().getProjectionMatrix();
		cameraView = m_cameraController.getCamera().getViewMatrix();
	}

	ImGuizmo::SetOrthographic(m_settings.cameraType == CameraType::Orthographic);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(
	    m_viewportInfo.min_bound.x, m_viewportInfo.min_bound.y,
	    m_viewportInfo.max_bound.x - m_viewportInfo.min_bound.x,
	    m_viewportInfo.max_bound.y - m_viewportInfo.min_bound.y);

	auto selectedEntity = m_hierarchyPanel->getSelectedEntity();

	// --- Manipulate selected entity

	if (selectedEntity && selectedEntity.has<TransformComponent>() && m_gizmoType != -1)
	{
		auto& tc = selectedEntity.get<hyp::TransformComponent>();
		glm::mat4 transform = tc.getTransform();

		bool snap = Input::isKeyPressed(Key::LEFT_CONTROL);
		float snapValue = 0.5f; // Snap to 0.5m for translation/scale
		// Snap to 45 degrees for rotation
		if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
			snapValue = 45.0f;

		float snapValues[] = { snapValue, snapValue, snapValue };

		ImGuizmo::Manipulate(
		    glm::value_ptr(cameraView),
		    glm::value_ptr(cameraProjection),
		    (ImGuizmo::OPERATION)m_gizmoType,
		    ImGuizmo::LOCAL,
		    glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

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

	ImGui::PopStyleVar();
	ImGui::End();

	render_toolbar();
}

void hyp::editor::EditorLayer::render_toolbar() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(m_iconPlay->getWidth() * 2.5f, m_iconPlay->getHeight()));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	auto& colors = ImGui::GetStyle().Colors;
	const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
	const auto& buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

	ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);

	bool toolbarEnabled = (bool)m_activeScene;

	ImVec4 tintColor = ImVec4(1, 1, 1, 1);
	if (!toolbarEnabled)
		tintColor.w = 0.5f;

	float size = ImGui::GetWindowHeight() - 4.f;
	ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

	bool hasPlayButton = m_sceneState == SceneState::Edit || m_sceneState == SceneState::Play;
	bool hasSimulateButton = m_sceneState == SceneState::Edit || m_sceneState == SceneState::Simulate;
	bool hasPauseButton = m_sceneState != SceneState::Edit;

	if (hasPlayButton)
	{
		Ref<Texture2D> icon = (m_sceneState == SceneState::Edit || m_sceneState == SceneState::Simulate) ? m_iconPlay : m_iconStop;
		if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->getTextureId(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
		{
			if (m_sceneState == SceneState::Edit || m_sceneState == SceneState::Simulate)
				onScenePlay();
			else if (m_sceneState == SceneState::Play)
				onSceneStop();
		}
	}

	if (hasPauseButton)
	{
		bool isPaused = m_activeScene->isPaused();
		ImGui::SameLine();
		{
			Ref<Texture2D> icon = isPaused ? m_iconPlay : m_iconPause;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->getTextureId(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_activeScene->setPause(!isPaused);
			}
		}
	}

	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(3);
	ImGui::End();
}

void hyp::editor::EditorLayer::onScenePlay() {
	if (m_sceneState == SceneState::Simulate)
		onSceneStop();

	m_sceneState = SceneState::Play;

	m_activeScene = hyp::Scene::copy(m_editorScene);
	m_activeScene->onRuntimeStart();

	m_hierarchyPanel->setContext(m_activeScene);
}

void hyp::editor::EditorLayer::onSceneStop() {
	HYP_ASSERT(m_sceneState == SceneState::Play || m_sceneState == SceneState::Simulate);

	if (m_sceneState == SceneState::Play) // stop runtime
		m_activeScene->onRuntimeStop();
	else if (m_sceneState == SceneState::Simulate) // stop simulation
		;

	m_sceneState = SceneState::Edit;

	m_activeScene = m_editorScene;

	m_hierarchyPanel->setContext(m_activeScene);
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

	bool control = hyp::Input::isKeyPressed(hyp::Key::LEFT_CONTROL) || hyp::Input::isKeyPressed(hyp::Key::RIGHT_CONTROL);
	bool shift = hyp::Input::isKeyPressed(hyp::Key::LEFT_SHIFT) || hyp::Input::isKeyPressed(hyp::Key::RIGHT_SHIFT);

	auto selectedEntity = m_hierarchyPanel->getSelectedEntity();

	//hmmm? should this be allowed outside of edit mode
	switch (event.getkey())
	{
	case hyp::Key::O:
	{
		if (m_sceneState != SceneState::Edit)
		{ // mitigates opening new scenes while playing/simulating a scene.
			HYP_WARN("Cannot open a scene outside Edit Mode");
			break;
		}

		if (control)
		{
			openScene();
		}
		break;
	}
	case hyp::Key::N: // create new scene
	{
		if (m_sceneState == SceneState::Edit && control)
		{
			newScene();
		}
		else
		{
			HYP_WARN("Cannot create a scene outside Edit Mode");
		}
		break;
	}
	case hyp::Key::Q:
	{
		if (!ImGuizmo::IsUsing())
		{
			m_gizmoType = -1;
		};
		break;
	}
	case hyp::Key::S:
	{
		if (control && shift)
		{ //Ctrl+Shift+S
			saveSceneAs();
		}
		if (control) // Ctrl+S
		{
			saveScene();
		}
		else if (selectedEntity && !ImGuizmo::IsUsing()) // scale
		{
			m_gizmoType = ImGuizmo::SCALE;
		}
		break;
	}
	case hyp::Key::R: // Rotate
	{
		if (selectedEntity && !ImGuizmo::IsUsing())
		{
			m_gizmoType = ImGuizmo::ROTATE;
		}
		break;
	}
	case hyp::Key::G: // translate
	{
		if (control && shift)
		{
			m_settings.showGrid = !m_settings.showGrid;
		}
		else if (selectedEntity && !ImGuizmo::IsUsing())
		{
			m_gizmoType = ImGuizmo::TRANSLATE;
		}
		break;
	}
	case hyp::Key::F2:
	{
		if (!m_settings.showHelpGuide)
		{
			m_settings.showHelpGuide = true;
		}
		break;
	}
	case hyp::Key::F5: //F5 to play and stop a scene
	{
		if (m_sceneState == SceneState::Edit || m_sceneState == SceneState::Simulate)
			onScenePlay();
		else if (m_sceneState == SceneState::Play)
			onSceneStop();
		break;
	}
	default:
		break;
	}

	return false;
}

void hyp::editor::EditorLayer::openScene() {
	std::string path = hyp::FileDialog::openFile("*.hypscene\0*.hyperscene", "Hyper Scene (*.hypscene)");

	if (!path.empty())
	{
		openScene(path);
	}
}

void hyp::editor::EditorLayer::openScene(const fs::path& path) {
	if (path.extension().string() != ".hypscene") // TODO: scene file identified by its extension?
	{
		HYP_WARN("Could not load %s - not a scene file", path.filename().c_str());
		return;
	}

	hyp::Ref<Scene> scene = hyp::CreateRef<Scene>();
	hyp::SceneSerializer serializer(scene);

	if (serializer.deserializer(path.string()))
	{
		m_editorScene = scene;
		m_hierarchyPanel->setContext(m_editorScene);

		m_activeScene = m_editorScene;
		m_settings.scenePath = path;
	}
}

void hyp::editor::EditorLayer::newScene() {
	m_activeScene = hyp::CreateRef<Scene>();
	m_hierarchyPanel->setContext(m_activeScene);

	m_settings.scenePath = fs::path();
}

void hyp::editor::EditorLayer::saveScene() {
	if (!m_settings.scenePath.empty())
	{
		serializerScene(m_activeScene, m_settings.scenePath);
	}
	else
		saveSceneAs();
}

void hyp::editor::EditorLayer::saveSceneAs() {
	std::string path = hyp::FileDialog::saveFile("*.hypscene\0", "Hyper Scene (*.hypscene)");

	if (!path.empty())
	{
		if (!fs::path(path).has_extension())
		{
			path += ".hypscene";
		}

		serializerScene(m_activeScene, path);
		m_settings.scenePath = path;
	}
}

void hyp::editor::EditorLayer::serializerScene(hyp::Ref<hyp::Scene> scene, const fs::path& path) {
	hyp::SceneSerializer serializer(scene);

	serializer.serializer(path.string());
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
