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

		class EditorLayer : public hyp::Layer {
		public:
			EditorLayer();

			virtual void onEvent(hyp::Event& event) override;
			virtual void onUpdate(float dt) override;
			virtual void onUIRender();

		private:
			bool onMousePressed(hyp::MouseBtnPressedEvent& event);
			bool onKeyPressed(hyp::KeyPressedEvent& event);
		private:
			hyp::Ref<hyp::Framebuffer> m_framebuffer;
			hyp::Ref<hyp::Scene> m_scene;
			hyp::Ref<hyp::HierarchyPanel> m_hierarchyPanel;

		private:
			ViewportState m_viewportInfo;

			int m_gizmoType = -1;

			hyp::Entity m_hoveredEntity;
			hyp::EditorCamera m_editorCamera;
		};
	};
}

#endif //! HYPER_EDITOR_LAYER
