#include <iostream>
#include <glad/glad.h>
#include "game_layer.hpp"
#include <GLFW/glfw3.h>
#include <renderer/shader.hpp>
#include <renderer/vertex_array.hpp>
#include <renderer/vertex_buffer.hpp>
#include <utils/logger.hpp>
#include <core/device.hpp>
#include <core/window.hpp>
#include <renderer/texture.hpp>
#include <core/application.hpp>
#include "camera.hpp"
#include <renderer/renderer2d.hpp>


class TestLayer : public hyp::Layer {
public:
	TestLayer(){
		
	}

	virtual void onAttach() override
	{
		glEnable(GL_DEPTH_TEST);
	}

	virtual void onDetach() override {
	}

	virtual void onUpdate(float dt) override {
		camera.onUpdate();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)600 / (float)600, 0.1f, 1000.0f);

		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;

		hyp::Renderer2D::beginScene(projection * camera.getViewMatrix());
		hyp::Renderer2D::drawQuad({ camX, 0.f,camZ }, { 50.f, 10.f }, { 0.f, 1.f, 0.f, 1.f });
		hyp::Renderer2D::drawQuad({0.f, 10.f,0.f}, {50.f, 10.f}, {0.f, 1.f, 0.5f, 1.f});
		hyp::Renderer2D::endScene();
	}

	bool onMouseBtnPressed(hyp::MouseBtnPressedEvent& event) {
		if (event.getButton() == hyp::Mouse::BUTTON_LEFT) {
			camera.hideCursor = true;
			glfwSetInputMode(hyp::Application::get().getWindow()->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		return false;
	}

	bool onMouseBtnReleased(hyp::MouseBtnReleasedEvent& event) {
		if (event.getButton() == hyp::Mouse::BUTTON_LEFT) {
			camera.hideCursor = false;
			glfwSetInputMode(hyp::Application::get().getWindow()->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		return false;
	}

	bool onMouseMoved(hyp::MouseMovedEvent& event) {
		return camera.onMouseMoved(event);
	}

	void onEvent(hyp::Event& event) {
		hyp::EventDispatcher ed(event);

		ed.dispatch<hyp::MouseBtnPressedEvent>(BIND_EVENT_FN(onMouseBtnPressed));
		ed.dispatch<hyp::MouseBtnReleasedEvent>(BIND_EVENT_FN(onMouseBtnReleased));
		ed.dispatch<hyp::MouseMovedEvent>(BIND_EVENT_FN(onMouseMoved));
	}


private:
	Camera camera;
};

int main(int argc, char** argv)
{
	hyp::Device::init({});
	hyp::WindowProps props("hyper-sandbox", 600, 600);
	props.resizable = false;

	auto app = hyp::Application(props);
	hyp::Renderer2D::init();

	app.pushLayer(new GameLayer({25.f, 125.f}));
	app.run();

	hyp::Renderer2D::deinit();
	hyp::Device::deinit();
	return 0;
};
