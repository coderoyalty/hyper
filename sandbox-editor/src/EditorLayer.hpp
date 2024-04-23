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

class EditorLayer : public hyp::Layer {
public:
	EditorLayer();

	virtual void onEvent(hyp::Event& event) override;
	virtual void onUpdate(float dt) override;
	virtual void onUIRender();

private:
	hyp::Ref<hyp::Framebuffer> m_framebuffer;
	hyp::Ref<hyp::Scene> m_scene;
	hyp::Ref<hyp::HierarchyPanel> m_hierarchyPanel;

private:
	glm::vec2 m_viewportSize;
	glm::vec2 m_viewportBounds[2];
	hyp::Ref<hyp::OrthoGraphicCameraController> m_cameraController;
	bool m_viewportFocused = false;
	hyp::Entity m_entity;
};

#endif //! HYPER_EDITOR_LAYER
