#include "editor_camera.hpp"
#include <utils/assert.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

hyp::EditorCamera::EditorCamera(float fov, float aspectRatio, float near, float far)
    : m_fov(fov), m_aspectRatio(aspectRatio),
      m_near(near), m_far(far) {
	HYP_ASSERT_CORE(m_aspectRatio != 0.f, "Aspect Ratio of 0.f is invalid");

	m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_near, m_far);
	updateView();
}

void hyp::EditorCamera::onUpdate(float dt) {
	if (hyp::Input::isKeyPressed(hyp::LEFT_ALT))
	{
		const glm::vec2 mousePos = hyp::Input::getMousePos();
		glm::vec2 delta = (mousePos - m_initialMousePos) * 0.003f;
		m_initialMousePos = mousePos;

		if (hyp::Input::isMouseBtnPressed(hyp::Mouse::BUTTON_MIDDLE))
		{
			mousePan(delta);
		}
		else if (hyp::Input::isMouseBtnPressed(hyp::Mouse::BUTTON_LEFT))
		{
			mouseRotate(delta);
		}
		else if (hyp::Input::isMouseBtnPressed(hyp::Mouse::BUTTON_RIGHT))
		{
			mouseZoom(delta.y);
		}
	}

	if (hyp::Input::isKeyPressed(hyp::Key::W))
	{
		m_focalPoint += getForwardDirection() * dt;
	}

	else if (hyp::Input::isKeyPressed(hyp::Key::S))
	{
		m_focalPoint -= getForwardDirection() * dt;
	}

	if (hyp::Input::isKeyPressed(hyp::Key::A))
	{
		m_focalPoint += getRightDirection() * dt;
	}

	else if (hyp::Input::isKeyPressed(hyp::Key::D))
	{
		m_focalPoint -= getRightDirection() * dt;
	}

	updateView();
}

void hyp::EditorCamera::onEvent(hyp::Event& e) {
	hyp::EventDispatcher ed(e);
	ed.dispatch<hyp::MouseScrolledEvent>(BIND_EVENT_FN(EditorCamera::onMouseScrolled));
}

glm::mat4 hyp::EditorCamera::getViewMatrix() const {
	return m_viewMatrix;
}

glm::mat4 hyp::EditorCamera::getViewProjectionMatrix() const {
	return m_projectionMatrix * m_viewMatrix;
}

glm::vec3 hyp::EditorCamera::getRightDirection() const {
	return glm::rotate(getOrientation(), glm::vec3(1.f, 0.f, 0.f));
}

glm::vec3 hyp::EditorCamera::getUpDirection() const {
	return glm::rotate(getOrientation(), glm::vec3(0.f, 1.f, 0.f));
}

glm::vec3 hyp::EditorCamera::getForwardDirection() const {
	return glm::rotate(getOrientation(), glm::vec3(0.f, 0.f, -1.f));
}

glm::quat hyp::EditorCamera::getOrientation() const {
	return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.f));
}

void hyp::EditorCamera::updateProjection() {
	m_aspectRatio = m_vWidth / m_vHeight;
	HYP_ASSERT_CORE(m_aspectRatio != 0.f, "Aspect Ratio of 0.f is invalid");
	this->m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_near, m_far);
}

void hyp::EditorCamera::updateView() {
	m_position = calcPosition();

	glm::quat orientation = getOrientation();
	m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
	m_viewMatrix = glm::inverse(m_viewMatrix);


	//m_viewMatrix = glm::lookAt(m_position, m_position + getForwardDirection(), getUpDirection());
}

glm::vec3 hyp::EditorCamera::calcPosition() {
	return m_focalPoint - getForwardDirection() * m_distance;
}

void hyp::EditorCamera::mousePan(const glm::vec2& delta) {
	auto [xSpeed, ySpeed] = panSpeed();
	m_focalPoint += -getRightDirection() * delta.x * xSpeed * m_distance;
	m_focalPoint += getUpDirection() * delta.y * ySpeed * m_distance;
}

void hyp::EditorCamera::mouseRotate(const glm::vec2& delta) {
	float rotationSpeed = 0.2f;
	float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
	m_yaw += yawSign * delta.x * rotationSpeed; // rotation speed
	m_pitch += delta.y * rotationSpeed;
}

void hyp::EditorCamera::mouseZoom(const float& delta) {
	m_distance -= delta * zoomSpeed();
	if (m_distance < 1.0f)
	{
		m_focalPoint += getForwardDirection();
		m_distance = 1.0f;
	}
}

std::pair<float, float> hyp::EditorCamera::panSpeed() {
	float x = std::min(m_vWidth / 1000.0f, 2.4f); // max = 2.4f
	float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

	float y = std::min(m_vHeight / 1000.0f, 2.4f); // max = 2.4f
	float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

	return { xFactor, yFactor };
}

float hyp::EditorCamera::zoomSpeed() const {
	float distance = m_distance * 0.2f;
	distance = std::max(distance, 0.0f);
	float speed = distance * distance;
	speed = std::min(speed, 100.0f); // max speed = 100
	return speed;
}

bool hyp::EditorCamera::onMouseScrolled(hyp::MouseScrolledEvent& ev) {
	float delta = ev.getYOffset() * 0.1f;
	mouseZoom(delta);
	updateView();
	return false;
}
