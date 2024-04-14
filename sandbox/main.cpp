#include <iostream>
#include <glad/glad.h>
#include "game_layer.hpp"
#include <GLFW/glfw3.h>
#include <utils/logger.hpp>
#include <core/device.hpp>
#include <core/window.hpp>
#include "game_layer.hpp"

int main(int argc, char** argv) {
	hyp::Device::init({});
	hyp::WindowProps props("hyper-sandbox: pong", 600, 600);
	props.resizable = false;

	auto app = hyp::Application(props);
	hyp::RenderCommand::init();
	hyp::Renderer2D::init();

	app.pushLayer(new GameLayer());
	app.run();

	hyp::Renderer2D::deinit();
	hyp::Device::deinit();
	return 0;
};
