#include <core/application.hpp>
#include <core/device.hpp>
#include <core/layer.hpp>
#include <iostream>
#include <renderer/renderer2d.hpp>
#include <io/input.hpp>
#include <renderer/orthographic_controller.hpp>
#include <imgui.h>


class TestLayer : public hyp::Layer {
public:

	TestLayer(float x, float y): camera(x,y) {

	}

	virtual void onEvent(hyp::Event& e) override {
		hyp::EventDispatcher ed(e);

		camera.onEvent(e);

		ed.dispatch<hyp::WindowResizeEvent>(BIND_EVENT_FN(onResize));
		ed.dispatch<hyp::KeyPressedEvent>(BIND_EVENT_FN(onKeyPressed));
	}

	virtual void onAttach() override {
	}

	virtual void onDetach() override {
	}

	virtual void onUpdate(float dt) override {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		camera.onUpdate(dt);

		if (toggleWireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		hyp::Renderer2D::beginScene(camera.getCamera().getViewProjectionMatrix());
		int num_rows = 5;

		for (int i = 0; i < num_rows; i++) {
			float red = i / static_cast<float>(num_rows);
			float green = 0.5f;
			float blue = 1.0f - red;

			hyp::Renderer2D::drawQuad({ i * (600.f / num_rows), 0.f, 0.f }, { 600.f / num_rows, 600.f / num_rows }, { red, green, blue, 1.0f });
		}

		float factor = 600.f / num_rows;

		hyp::Renderer2D::drawLine({ 0.f, 0.f, 0.f }, { factor, 0.f, 0.f }, glm::vec4(0.28, 0.75f, 0.f, 1.f));
		hyp::Renderer2D::drawLine({ factor, 0.f, 0.f }, { factor, factor, 0.f }, glm::vec4(1.f, 1.f, 0.f, 1.f));
		hyp::Renderer2D::drawLine({ 0.f, factor, 0.f }, { factor, factor, 0.f }, glm::vec4(0.f, 1.f, 1.f, 1.f));
		hyp::Renderer2D::drawLine({ 0.f, 0.f, 0.f }, {0.f, factor, 0.f}, glm::vec4(1.f, 0.f, 1.f, 1.f));

		hyp::Renderer2D::endScene();
	}

	bool onKeyPressed(hyp::KeyPressedEvent& event) {
		if (event.getkey() == hyp::Key::TAB && !event.isRepeat()) {
			toggleWireframe = !toggleWireframe;
		}

		return false;
	}

	bool onResize(hyp::WindowResizeEvent& event) {
		float w = (float)event.getWidth();
		float h = (float)event.getHeight();
		glViewport(0, 0, w, h);

		return false;
	}

	bool toggleWireframe = false;

	bool show_demo_window = false;
	bool show_another_window = false;
	int counter = 0;

	virtual void onUIRender() override {

		auto& io = ImGui::GetIO();

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

private:
	hyp::OrthoGraphicCameraController camera;
};

int main(int argc, char** argv) {
	hyp::Device::init({});

	hyp::WindowProps props("hyper-editor", 600, 600);

	hyp::Application app(props);
	hyp::Renderer2D::init();

	hyp::Layer* layer = new TestLayer(600.f, 600.f);

	app.pushLayer(layer);

	app.run();

	hyp::Renderer2D::deinit();
	hyp::Device::deinit();
}