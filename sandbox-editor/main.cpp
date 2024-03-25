#include <core/application.hpp>
#include <core/device.hpp>
#include <core/layer.hpp>
#include <iostream>
#include <renderer/renderer2d.hpp>
#include <io/input.hpp>


class TestLayer : public hyp::Layer {
public:
	virtual void onEvent(hyp::Event& e) override {
		hyp::EventDispatcher ed(e);
		ed.dispatch<hyp::WindowResizeEvent>(BIND_EVENT_FN(onResize));
		ed.dispatch<hyp::KeyPressedEvent>(BIND_EVENT_FN(onKeyPressed));
	}

	virtual void onAttach() override {
		viewProjMat = glm::ortho(0.f, (float)600.f, (float)600.f, 0.f, -1.f, 1.f);
	}

	virtual void onDetach() override {
	}

	virtual void onUpdate(float dt) override {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (toggleWireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		hyp::Renderer2D::beginScene(viewProjMat);
		int num_rows = 5;

		for (int i = 0; i < num_rows; i++) {
			float red = i / static_cast<float>(num_rows);
			float green = 0.5f;
			float blue = 1.0f - red;

			hyp::Renderer2D::drawQuad({ i * (600.f / num_rows), 0.f, 0.f }, { 600.f / num_rows, 600.f / num_rows }, { red, green, blue, 1.0f });
		}

		hyp::Renderer2D::endScene();
	}

	bool onKeyPressed(hyp::KeyPressedEvent& event) {
		if (event.getkey() == hyp::Key::TAB && !event.isRepeat()) {
			toggleWireframe = !toggleWireframe;
		}

		return false;
	}

	bool onResize(hyp::WindowResizeEvent& event) {
		glViewport(0, 0, event.getWidth(), event.getHeight());

		viewProjMat = glm::ortho(0.f, (float)event.getWidth(), (float)event.getHeight(), 0.f, -1.f, 1.f);

		return false;
	}

	bool toggleWireframe = false;

private:
	glm::mat4 viewProjMat = glm::mat4(1.0);
};

int main(int argc, char** argv) {
	hyp::Device::init({});

	hyp::WindowProps props("hyper-editor", 600, 600);

	hyp::Application app(props);
	hyp::Renderer2D::init();

	hyp::Layer* layer = new TestLayer();

	app.pushLayer(layer);

	app.run();

	hyp::Renderer2D::deinit();
	hyp::Device::deinit();
}