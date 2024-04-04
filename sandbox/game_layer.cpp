#include "game_layer.hpp"
#include <renderer/renderer2d.hpp>

constexpr float space = 5.f;

GameLayer::GameLayer(const glm::vec2& size)
{
	win_width = hyp::Application::get().getWindow()->getWidth();
	win_height = hyp::Application::get().getWindow()->getHeight();

	m_controller = hyp::CreateRef<hyp::OrthoGraphicCameraController>(win_width, win_height);

	float centerY = win_height / 2.f - size.y / 2.f;

	this->paddleLeft.size = size;
	this->paddleRight.size = size;


	this->paddleLeft.position.x = space;
	this->paddleLeft.position.y = centerY;

	this->paddleRight.position.x = win_width - size.x - space;
	this->paddleRight.position.y = centerY;
}

void GameLayer::onAttach()
{
}

void GameLayer::onDetach()
{
}

void GameLayer::onUpdate(float dt)
{
	/* left paddle */

	if (movingUp) {
		if (paddleLeft.position.y - 200.f * dt < 5.f) {
			movingUp = !movingUp;
		}
		else {
			paddleLeft.position.y -= 200.f * dt;
		}
	}
	else {
		if (paddleLeft.position.y + 20.f * dt >= win_height - paddleLeft.size.y - space) {
			movingUp = !movingUp;
		}
		else {
			paddleLeft.position.y += 200.f * dt;
		}
	}

	onRender();
}

void GameLayer::onRender()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	hyp::Renderer2D::beginScene(m_controller->getCamera().getViewProjectionMatrix());

	hyp::Renderer2D::drawQuad(glm::vec3(paddleLeft.position, 0.f), paddleLeft.size, glm::vec4(0.5f, 0.f, 0.f, 1.f));
	hyp::Renderer2D::drawQuad(glm::vec3(paddleRight.position, 0.f), paddleRight.size, glm::vec4(0.f, 0.f, 0.5f, 1.f));

	hyp::Renderer2D::endScene();
}
