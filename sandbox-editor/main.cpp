#include <core/application.hpp>
#include <core/device.hpp>
#include <core/layer.hpp>
#include <iostream>

class TestLayer: public hyp::Layer {
public:
	virtual void onEvent(hyp::Event& e) override {
		hyp::EventDispatcher ed(e);
		ed.dispatch<hyp::WindowResizeEvent>(BIND_EVENT_FN(onResize));
		ed.dispatch<hyp::WindowCloseEvent>(BIND_EVENT_FN(onClose));
	}
private:
	bool onResize(hyp::WindowResizeEvent& event) {
		std::cout << event.toString() << std::endl;
		return true;
	}

	bool onClose(hyp::WindowCloseEvent& event) {
		std::cout << "window closed" << std::endl;
		return true;
	}
};

int main(int argc, char** argv) {
	hyp::Device::init({});

	hyp::WindowProps props("hyper-editor", 600, 600);

	hyp::Application app(props);

	hyp::Layer* layer = new TestLayer();

	app.pushLayer(layer);

	app.run();

	hyp::Device::deinit();
}