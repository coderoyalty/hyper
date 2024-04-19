#pragma once
#include "GameLayer.hpp"

GameLayer::GameLayer(const glm::vec2& viewport) {
	v_width = viewport.x;
	v_height = viewport.y;

	glm::vec2 size(5.f, 75.f);

	m_controller = hyp::CreateRef<hyp::OrthoGraphicCameraController>(v_width, v_height);

	float centerX = v_width / 2.f;
	float centerY = v_height / 2.f;

	this->paddleLeft.size = size;
	this->paddleRight.size = size;

	this->paddleLeft.position.x = space;
	this->paddleLeft.position.y = centerY - size.y / 2.f;

	this->paddleRight.position.x = v_width - size.x - space;
	this->paddleRight.position.y = centerY - size.y / 2.f;

	this->ball.radius = 10.f;
	this->ball.velocity = INITIAL_VELOCITY;
	this->ball.position.x = centerX - this->ball.radius / 2.f;
	this->ball.position.y = centerY - this->ball.radius / 2.f;

	hyp::RenderCommand::setLineWidth(4.f);
}

void GameLayer::onUpdate(float dt) {
	if (!isPaused)
	{
		ball.update(dt);
		handleCollision();
		movePlayers(dt);
	}
	else
	{
		m_controller->onUpdate(dt);
	}

	onRender();
}

void GameLayer::onUIRender() {
#ifndef HYPER_DEBUG
	// don't render the UI in release mode!
	return;
#endif
	ImGui::Begin("Application");
	ImGui::Text("Draw Calls: %d", hyp::Renderer2D::getStats().drawCalls);
	ImGui::Text("No. Quads: %d", hyp::Renderer2D::getStats().getQuadCount());
	ImGui::Text("No. Lines: %d", hyp::Renderer2D::getStats().getLineCount());
	// unfortunately the current hyper API is does not have stats for circle's created..

	//game stats
	// (hurray! deprecated because Hyper support text rendering)
	ImGui::Text("Player 1 Score: %d", stat.score1);
	ImGui::Text("Player 2 Score: %d", stat.score2);
	if (ImGui::Button("Reset Stats"))
	{
		stat.reset();
	}

	// toggle the game state
	if (ImGui::Button("Toggle Game Pause"))
	{
		isPaused = !isPaused;

		ImGui::SameLine();
		ImGui::Text(isPaused ? "Click to Play" : "Click to Pause");
	}

	float size[2] = { paddleLeft.size.x, paddleLeft.size.y };

	// adjust paddle size

	ImGui::DragFloat2("Paddle Size", size, 0.1, 5.0, 100);

	paddleLeft.size = { size[0], size[1] };
	paddleRight.size = { size[0], size[1] };

	// adjust ball parameters
	ImGui::Text("Ball Info");
	ImGui::DragFloat("Ball Radius", &ball.radius, 0.1, 10.0, 30.f);
	ImGui::DragFloat("Ball Fade", &ball.fade, 0.01, 0.0, 1.0);
	ImGui::DragFloat("Ball Thickness", &ball.thickness, 0.01, 0.0, 1.0);

	// tweak viewport here!
	if (ImGui::DragFloat("Viewport Width", &v_width, 0.1, 200.f, 1000.f))
	{
		m_controller->onResize(v_width, v_height);

		// update the right paddle to the end of the viewport
		this->paddleRight.position.x = v_width - paddleRight.size.x - space;
	}
	if (ImGui::DragFloat("Viewport Height", &v_height, 0.1, 300.f, 9000.f))
	{
		m_controller->onResize(v_width, v_height);
	}

	ImGui::End();
}

void GameLayer::onRender() {
	hyp::RenderCommand::clear();
	hyp::RenderCommand::setClearColor(0.3, 0.3, 0.3, 1.f);

	hyp::Renderer2D::beginScene(m_controller->getCamera().getViewProjectionMatrix());

	// draw the game score
	glm::vec2 size(50.f);
	glm::vec2 position((v_width * 0.5f) - (size.x * 0.278f), 25.f);

	hyp::Renderer2D::TextParams textParam;
	textParam.fontSize = 16.f;
	textParam.color = glm::vec4(0.0, 1.0, 0.f, 1.f);

	glm::mat4 model(1.0);
	model = glm::translate(model, glm::vec3(position + glm::vec2(0.f, textParam.fontSize), 1.f));
	model = glm::scale(model, glm::vec3(size, 0.f));

	std::stringstream ss;
	ss << stat.score1 << ":" << stat.score2 << std::endl;

	hyp::Renderer2D::drawString(ss.str(), hyp::Font::getDefault(), model, textParam);

	if (isPaused)
	{
		model = glm::translate(glm::mat4(1.0), glm::vec3(50.f, v_height * 0.5 - textParam.fontSize, 1.f));
		model = glm::scale(model, glm::vec3(size, 0.f));

		textParam.color = glm::vec4(1.f, 0.5f, 0.f, 1.f);
		hyp::Renderer2D::drawString("Press Space to Resume!", hyp::Font::getDefault(), model, textParam);
	}

	// draw the paddles
	hyp::Renderer2D::drawQuad(glm::vec3(paddleLeft.position, 0.f), paddleLeft.size, glm::vec4(1.f, 0.f, 1.f, 1.f));
	hyp::Renderer2D::drawQuad(glm::vec3(paddleRight.position, 0.f), paddleRight.size, glm::vec4(0.f, 0.f, 1.f, 1.f));

	// draw the camera viewport here
	glm::vec3 pos = m_controller->getCamera().getPosition();
	drawRect(glm::vec3(v_width * 0.5f, v_height * 0.5f, 0.f), glm::vec2(v_width, v_height), glm::vec4(1.0, 0.0, 0.0, 1.f));

	/* Draw the Nets */

	int length = 10;
	int spacing = 5;
	int numLines = v_height / (length + 2 * spacing);
	for (int i = 0; i < numLines; i++)
	{
		glm::vec3 p1 = glm::vec3(v_width / 2.f, i * (length + 2 * spacing) + spacing, 0.f);
		glm::vec3 p2 = glm::vec3(0.f, length, 0.f);
		hyp::Renderer2D::drawLine(p1, p1 + p2, glm::vec4(1.0, 1.0, 0.f, 1.f));
	}

	glm::vec2 halfWindow = { v_width * 0.5f, v_height };

	// viewport background
	hyp::Renderer2D::drawQuad(glm::vec3(0.f, 0.f, -0.5f), halfWindow, glm::vec4(1.f));
	hyp::Renderer2D::drawQuad(glm::vec3(halfWindow.x, 0.f, -0.5f), halfWindow, glm::vec4(0.f, 0.f, 0.f, 1.f));

	if (ball.position.x >= halfWindow.x)
	{
		ball.draw(glm::vec4(1.f));
	}
	else
	{
		ball.draw(glm::vec4(0.f, 0.f, 0.f, 1.f));
	}

	hyp::Renderer2D::endScene();
}

bool GameLayer::checkPaddleCollision(const Ball& ball, const Paddle& paddle) const {
	float ballLeft = ball.position.x;
	float ballRight = ball.position.x + ball.radius;
	float ballTop = ball.position.y;
	float ballBottom = ball.position.y + ball.radius;

	float paddleLeft = paddle.position.x;
	float paddleRight = paddle.position.x + paddle.size.x;
	float paddleTop = paddle.position.y;
	float paddleBottom = paddle.position.y + paddle.size.y;

	return ballLeft <= paddleRight &&
	       ballRight >= paddleLeft &&
	       ballTop <= paddleBottom &&
	       ballBottom >= paddleTop;
}

void GameLayer::movePlayers(float dt) {
	float delta_distance = INITIAL_VELOCITY * dt;
	if (hyp::Input::isKeyPressed(hyp::Key::W) && paddleLeft.position.y - delta_distance > space)
	{
		paddleLeft.position.y -= delta_distance;
	}
	else if (hyp::Input::isKeyPressed(hyp::Key::S) &&
	         paddleLeft.position.y + delta_distance < v_height - paddleLeft.size.y - space)
	{
		paddleLeft.position.y += delta_distance;
	};

	if (hyp::Input::isKeyPressed(hyp::Key::UP) && paddleRight.position.y - delta_distance > space)
	{
		paddleRight.position.y -= delta_distance;
	}
	else if (hyp::Input::isKeyPressed(hyp::Key::DOWN) &&
	         paddleRight.position.y + delta_distance < v_height - paddleLeft.size.y - space)
	{
		paddleRight.position.y += delta_distance;
	};
}

void GameLayer::handleCollision(Paddle& paddle, Ball& ball) {
	float t = ((ball.position.y - paddle.position.y) / paddle.size.y) - 0.5f;
	ball.dir.y = t;
}

void GameLayer::handleWallCollision() {
	float ballLeft = ball.position.x;
	float ballRight = ball.position.x + ball.radius;
	float ballTop = ball.position.y;
	float ballBottom = ball.position.y + ball.radius;

	if (ballTop <= space)
	{
		ball.dir.y = fabs(ball.dir.x);
	}

	if (ballBottom >= v_height - space)
	{
		ball.dir.y = -fabs(ball.dir.x);
	}

	float centerX = v_width / 2.f;
	float centerY = v_height / 2.f;

	glm::vec2 size = paddleLeft.size;

	if (ballLeft <= space)
	{
		ball.position.x = v_width * 0.5f - ball.radius * 0.5f;
		ball.position.y = v_height * 0.5f - ball.radius * 0.5f;
		ball.dir.x = fabs(ball.dir.x);
		ball.dir.y = 0;
		stat.score2++;

		this->paddleLeft.position.x = space;
		this->paddleLeft.position.y = centerY - size.y / 2.f;

		this->paddleRight.position.x = v_width - size.x - space;
		this->paddleRight.position.y = centerY - size.y / 2.f;
	}
	else if (ballRight >= v_height - space)
	{
		ball.position.x = v_width * 0.5f - ball.radius * 0.5f;
		ball.position.y = v_height * 0.5f - ball.radius * 0.5f;
		ball.dir.x = -fabs(ball.dir.x);
		ball.dir.y = 0;
		stat.score1++;

		this->paddleLeft.position.x = space;
		this->paddleLeft.position.y = centerY - size.y / 2.f;

		this->paddleRight.position.x = v_width - size.x - space;
		this->paddleRight.position.y = centerY - size.y / 2.f;
	}
}

void GameLayer::handleCollision() {
	handleWallCollision();
	if (checkPaddleCollision(ball, paddleLeft))
	{
		ball.dir.x = fabs(ball.dir.x);
		handleCollision(paddleLeft, ball);
	}

	if (checkPaddleCollision(ball, paddleRight))
	{
		ball.dir.x = -fabs(ball.dir.x);
		handleCollision(paddleRight, ball);
	}

	ball.dir = glm::normalize(ball.dir);
}