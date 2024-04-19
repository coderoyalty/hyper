#pragma once

#pragma once
#include <core/application.hpp>
#include <renderer/orthographic_controller.hpp>
#include <renderer/renderer2d.hpp>
#include <imgui.h>


constexpr float space = 5.f;
const float INITIAL_VELOCITY = 175.f;

struct Paddle
{
	glm::vec2 position;
	glm::vec2 size;
};

struct Ball
{
	glm::vec2 position {};
	float radius = 5.f;
	float velocity;
	glm::vec2 dir { 1.f, 0.f };

	float thickness = 1.f;
	float fade = 0.f;

	void update(float dt) {
		position += dir * velocity * dt;
	}

	void draw(const glm::vec4& color = glm::vec4(1.0)) {
		glm::mat4 transform = glm::translate(glm::mat4(1.0), glm::vec3(position.x + radius * 0.5f, position.y + radius * 0.5f, 0.f));
		transform = glm::scale(transform, glm::vec3(radius, radius, 1.f));
		hyp::Renderer2D::drawCircle(transform, thickness, fade, color);
	}
};

struct GameStats
{
	int score1 = 0;
	int score2 = 0;

	void reset() {
		score1 = score2 = 0;
	}
};

class GameLayer : public hyp::Layer {
public:
	GameLayer(const glm::vec2& viewport);

	virtual void onUpdate(float dt) override;

	virtual void onEvent(hyp::Event& e) {
		m_controller->onEvent(e);

		hyp::EventDispatcher ed(e);
		ed.dispatch<hyp::KeyPressedEvent>(BIND_EVENT_FN(onKeyPressed));
	};

	bool onKeyPressed(hyp::KeyPressedEvent& event) {
		if (event.getkey() == hyp::Key::SPACE && !event.isRepeat())
		{
			isPaused = !isPaused;
		}

		return false;
	}

	virtual void onUIRender() override;

	void onRender();

private:
	void movePlayers(float dt);

	void handleCollision(Paddle& paddle, Ball& ball);
	bool checkPaddleCollision(const Ball& ball, const Paddle& paddle) const;
	void handleWallCollision();
	void handleCollision();

	void drawRect(glm::vec3 position, glm::vec2 size, glm::vec4 color) {
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		hyp::Renderer2D::drawLine(p0, p1, color);
		hyp::Renderer2D::drawLine(p1, p2, color);
		hyp::Renderer2D::drawLine(p2, p3, color);
		hyp::Renderer2D::drawLine(p3, p0, color);
	}

private:
	Paddle paddleLeft;
	Paddle paddleRight;
	Ball ball;

	hyp::Shared<hyp::OrthoGraphicCameraController> m_controller;
	float v_width;
	float v_height;

	bool isPaused = true;

	GameStats stat;
};