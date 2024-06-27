#pragma once
#ifndef HYPER_EDITOR_LAYER
	#define HYPER_EDITOR_LAYER
	#include <core/application.hpp>
	#include <glm/glm.hpp>
	#include <renderer/framebuffer.hpp>
	#include <camera/orthographic_controller.hpp>
	#include <scene/components.hpp>
	#include <scene/entity.hpp>
	#include "panels/hierarchyPanel.hpp"
	#include <camera/editor_camera.hpp>
	#include <filesystem>
	#include <renderer/shader.hpp>
	#include <renderer/vertex_array.hpp>
	#include <renderer/vertex_buffer.hpp>
	#include "panels/contentBrowser.hpp"

namespace fs = std::filesystem;

namespace hyp {
	namespace editor {

		struct ViewportState
		{
			bool hovered = false;
			bool focused = false;

			glm::vec2 size = { 0.f, 0.f };

			union
			{
				struct
				{
					glm::vec2 min_bound;
					glm::vec2 max_bound;
				};
				glm::vec2 bounds[2] = { { 0.f, 0.f }, { 0.f, 0.f } };
			};
		};

		enum class CameraType
		{
			Perspective,
			Orthographic,
		};

		struct EditorSettings
		{
			bool showHelpGuide = false;
			bool showGrid = true;
			fs::path scenePath;

			CameraType cameraType = CameraType::Perspective;
			//TODO:
			// bool antialiased = false
			//
		};

		class EditorLayer : public hyp::Layer {
		public:
			EditorLayer();

			virtual void onAttach() override;
			virtual void onDetach() override;
			virtual void onEvent(hyp::Event& event) override;
			virtual void onUpdate(float dt) override;
			virtual void onUIRender();

		private:
			bool onMousePressed(hyp::MouseBtnPressedEvent& event);
			bool onKeyPressed(hyp::KeyPressedEvent& event);

		private:
			void openScene();
			void openScene(const fs::path& path);
			void newScene();
			void saveScene();
			void saveSceneAs();
			void serializerScene(hyp::Ref<hyp::Scene> scene, const fs::path& path);

		private:
			void render_toolbar();

			void onScenePlay();
			void onSceneStop();

			enum class SceneState
			{
				Play,
				Edit,
				Simulate,
				Stop
			};

			SceneState m_sceneState = SceneState::Edit;

			hyp::Ref<hyp::Texture2D> m_iconPlay;
			hyp::Ref<hyp::Texture2D> m_iconStop;
			hyp::Ref<hyp::Texture2D> m_iconPause;

		private:
			hyp::Ref<hyp::Framebuffer> m_framebuffer;
			hyp::Ref<hyp::Scene> m_editorScene;
			hyp::Ref<hyp::Scene> m_activeScene; // mostly used for interacting with the outside world :)
			hyp::Ref<hyp::HierarchyPanel> m_hierarchyPanel;
			hyp::Scope<ContentBrowserPanel> m_contentBrowserPanel;

			hyp::Ref<hyp::ShaderProgram> m_gridProgram;
			hyp::Ref<hyp::VertexArray> m_gridVao;

		private:
			ViewportState m_viewportInfo;
			EditorSettings m_settings;

			int m_gizmoType = -1;

			hyp::Entity m_hoveredEntity;

			hyp::EditorCamera m_editorCamera;
			hyp::OrthoGraphicCameraController m_cameraController;
		};
	};
}

#endif //! HYPER_EDITOR_LAYER
