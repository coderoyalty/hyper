#pragma once
#ifndef HYPER_EDITOR_LAYER
#define HYPER_EDITOR_LAYER
#include <core/application.hpp>
#include <glm/glm.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/orthographic_controller.hpp>

class EditorLayer : public hyp::Layer {
public:
	EditorLayer();

	virtual void onEvent(hyp::Event& event) override;
	virtual void onUpdate(float dt) override;
	virtual void onUIRender();

private:
	hyp::Ref<hyp::Framebuffer> m_framebuffer;
	glm::vec2 m_viewportSize;
	hyp::Ref<hyp::OrthoGraphicCameraController> m_cameraController;

	bool m_viewportFocused = false;
};

#endif //! HYPER_EDITOR_LAYER
