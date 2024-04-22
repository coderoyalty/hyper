#include "orthographic_controller.hpp"
#include <iostream>

hyp::OrthoGraphicCameraController::OrthoGraphicCameraController(float vx, float vy)
    : m_aspectRatio(vx / vy),
      m_camera(0, vx, vy, 0),
      m_width(vx), m_height(vy) {
	m_camera.setPosition(glm::vec3(0.f));
}

void hyp::OrthoGraphicCameraController::onEvent(hyp::Event& event) {
	hyp::EventDispatcher ed(event);

	ed.dispatch<hyp::WindowResizeEvent>(BIND_EVENT_FN(onWindowResized));
	ed.dispatch<hyp::MouseScrolledEvent>(BIND_EVENT_FN(onMouseScrolled));
}

void hyp::OrthoGraphicCameraController::onUpdate(float dt) {
	glm::vec3 cameraPos = m_camera.getPosition();

	if (hyp::Input::isKeyPressed(hyp::Key::W))
	{
		cameraPos -= glm::vec3(0.0f, m_cameraSpeed * dt, 0.0f);
	}
	else if (hyp::Input::isKeyPressed(hyp::Key::S))
	{
		cameraPos += glm::vec3(0.0f, m_cameraSpeed * dt, 0.0f);
	}

	if (hyp::Input::isKeyPressed(hyp::Key::A))
	{
		cameraPos -= glm::vec3(m_cameraSpeed * dt, 0.0f, 0.0f);
	}
	else if (hyp::Input::isKeyPressed(hyp::Key::D))
	{
		cameraPos += glm::vec3(m_cameraSpeed * dt, 0.0f, 0.0f);
	}

	m_camera.setPosition(cameraPos);

	float cameraRotation = m_camera.getRotation();

	if (hyp::Input::isKeyPressed(hyp::Key::Q))
	{
		cameraRotation += m_cameraRotationSpeed * dt;
	}
	else if (hyp::Input::isKeyPressed(hyp::Key::E))
	{
		cameraRotation -= m_cameraRotationSpeed * dt;
	}

	if (cameraRotation > 180.0f)
		cameraRotation -= 360.0f;
	else if (cameraRotation <= -180.0f)
		cameraRotation += 360.0f;

	m_camera.setRotation(cameraRotation);
}

void hyp::OrthoGraphicCameraController::onResize(float width, float height) {
	if (height == 0.f)
	{
		return;
	}

	m_aspectRatio = width / height;
	m_width = width;
	m_height = height;
	m_camera.setProjection(0, width, height, 0);
}

bool hyp::OrthoGraphicCameraController::onMouseScrolled(hyp::MouseScrolledEvent& event) {
	m_zoomLevel -= event.getYOffset() * 0.25f;
	m_zoomLevel = std::max(m_zoomLevel, 0.25f);

	m_zoomLevel = std::min(m_zoomLevel, 12.5f);

	float half_width = m_width / 2.f;
	float half_height = m_height / 2.f;
	m_camera.setProjection(0.f, m_width * m_zoomLevel, m_height * m_zoomLevel, 0.f);

	return false;
}

bool hyp::OrthoGraphicCameraController::onWindowResized(hyp::WindowResizeEvent& e) {
	onResize((float)e.getWidth(), (float)e.getHeight());
	return false;
}
