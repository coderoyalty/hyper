#include "pch.h"
#include "orthographic_controller.hpp"
#include <iostream>

hyp::OrthoGraphicCameraController::OrthoGraphicCameraController(float vx, float vy, OrthoGraphicOnProjectionCB projection)
    : m_aspectRatio(vx / vy),
      m_camera(0, vx, vy, 0),
      m_width(vx), m_height(vy),
      m_projectionCB(projection) {
	m_camera.setPosition(glm::vec3(0.f));

	if (!m_projectionCB)
	{
		m_camera.setProjection(m_zoomLevel * -m_aspectRatio, m_zoomLevel * m_aspectRatio, -m_zoomLevel, m_zoomLevel);
	}
	else
	{
		(*m_projectionCB)(this, (float)m_width, (float)m_height);
	}
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

	// use the callback to update the projection instead
	if (m_projectionCB)
	{
		(*m_projectionCB)(this, (float)m_width, (float)m_height);
		return;
	}

	m_camera.setProjection(m_zoomLevel * -m_aspectRatio, m_zoomLevel * m_aspectRatio, -m_zoomLevel, m_zoomLevel);
}

void hyp::OrthoGraphicCameraController::setProjectionCallBack(OrthoGraphicOnProjectionCB cb) {
	m_projectionCB = cb;
}

bool hyp::OrthoGraphicCameraController::onMouseScrolled(hyp::MouseScrolledEvent& event) {
	m_zoomLevel -= event.getYOffset() * 0.25f;
	m_zoomLevel = std::max(m_zoomLevel, 0.25f);

	m_zoomLevel = std::min(m_zoomLevel, 12.5f);

	if (m_projectionCB)
	{
		(*m_projectionCB)(this, (float)m_width, (float)m_height);
		return false;
	}

	m_camera.setProjection(m_zoomLevel * -m_aspectRatio, m_zoomLevel * m_aspectRatio, -m_zoomLevel, m_zoomLevel);

	return false;
}

bool hyp::OrthoGraphicCameraController::onWindowResized(hyp::WindowResizeEvent& e) {
	onResize((float)e.getWidth(), (float)e.getHeight());
	return false;
}
