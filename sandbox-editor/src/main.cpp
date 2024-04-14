#include <core/application.hpp>
#include <core/device.hpp>
#include <renderer/renderer2d.hpp>
#include <renderer/render_command.hpp>
#include "EditorLayer.hpp"

int main(int argc, char** argv) {
	hyp::Device::init({});

	hyp::WindowProps props("hyper-editor", 600, 600);
	props.maximized = true;

	hyp::Application app(props);
	hyp::RenderCommand::init();
	hyp::Renderer2D::init();

	app.pushLayer(new EditorLayer());

	app.run();

	hyp::Renderer2D::deinit();
	hyp::Device::deinit();
}