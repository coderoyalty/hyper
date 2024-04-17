#pragma once

#include <core/application.hpp>
#include <renderer/orthographic_controller.hpp>
#include <renderer/renderer2d.hpp>
#include <renderer/font.hpp>

#define WIDTH 600
#define HEIGHT 600


class GameLayer : public hyp::Layer {
public:
	virtual void onEvent(hyp::Event& event) {}

	virtual void onUpdate(float dt) override;

	virtual void onAttach() override;

	virtual void onUIRender() override;
private:
	hyp::Ref<hyp::OrthoGraphicCameraController> m_cameraController;
};
