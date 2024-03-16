#include <core/application.hpp>
#include <core/device.hpp>

int main(int argc, char** argv) {
	hyp::Device::init({});

	hyp::WindowProps props("hyper-editor", 600, 600);

	hyp::Application app(props);

	app.run();

	hyp::Device::deinit();
}