#include "orthographic_camera.hpp"

hyp::OrthoGraphicCamera::OrthoGraphicCamera(float l, float r, float b, float t) : m_viewMatrix(1.0f) {
	setProjection(l, r, b, t);
}

void hyp::OrthoGraphicCamera::setProjection(float l, float r, float b, float t) {
	m_projectionMatrix = glm::ortho(l, r, b, t, -1.f, 1.f);
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void hyp::OrthoGraphicCamera::recalculateViewMatrix() {
	glm::mat4 transform = glm::translate(glm::mat4(1.0), m_position) * glm::rotate(glm::mat4(1.0),
	                                                                       glm::radians(m_rotation),
	                                                                       { 0.f, 0.f, 1.f });

	m_viewMatrix = glm::inverse(transform);
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}
