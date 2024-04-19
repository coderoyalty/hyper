#include <iostream>
#include "GameLayer.hpp"
#include <utils/logger.hpp>
#include <core/device.hpp>
#include <core/application.hpp>

int main(int argc, char** argv) {
	hyp::Device::init({});
	hyp::WindowProps props("hyper-sandbox: phong", 600, 600);
	props.resizable = false;

	auto app = hyp::Application(props);
	hyp::RenderCommand::init();
	hyp::Renderer2D::init();

	app.pushLayer(new GameLayer({ 300.f, 300.f }));
	app.run();

	hyp::Renderer2D::deinit();
	hyp::Device::deinit();
	return 0;
};