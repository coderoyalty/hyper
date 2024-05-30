#include "pch.h"
#include "scene_camera.hpp"

hyp::SceneCamera::SceneCamera() {
	m_fov = glm::radians(45.f);

	recalculateProjection();
}

void hyp::SceneCamera::setPerspective(float fov, float nearClip, float farClip) {
	m_far = farClip;
	m_near = nearClip;
	m_fov = fov;

	setProjectionType(ProjectionType::Perspective);
}

void hyp::SceneCamera::setOrthographic(float size, float nearClip, float farClip) {
	m_size = size;
	m_orthoNear = nearClip;
	m_orthoFar = farClip;

	setProjectionType(ProjectionType::OrthoGraphic);
}

void hyp::SceneCamera::setViewport(uint32_t width, uint32_t height) {
	HYP_ASSERT(width > 0 && height > 0);

	m_aspectRatio = (float)width / (float)height;
	recalculateProjection();
}

void hyp::SceneCamera::recalculateProjection() {
	if (m_projectionType == ProjectionType::Perspective)
	{
		m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_near, m_far);
	}
	else
	{
		float left = -m_size * m_aspectRatio * 0.5f;
		float right = m_size * m_aspectRatio * 0.5f;
		float bottom = -m_size * 0.5f;
		float top = m_size * 0.5f;

		m_projectionMatrix = glm::ortho(left, right, bottom, top);
	}
}
