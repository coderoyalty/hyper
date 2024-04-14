#pragma once

#include <glm/glm.hpp>
#include <event/event.hpp>
#include <event/key_event.hpp>
#include <event/mouse_event.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <io/input.hpp>
class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f)
	    : position(position), worldUp(up), yaw(yaw), pitch(pitch),
	      front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(0.05f), mouseSensitivity(0.1f), zoom(45.0f) {
		update();
	}

	//void onEvent(hyp::Event& event) {
	//	hyp::EventDispatcher ed(event);
	//	ed.dispatch<hyp::KeyPressedEvent>(BIND_EVENT_FN(onKeyPressed));
	//}

	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}

	bool onUpdate() {
		if (hyp::Input::isKeyPressed(hyp::Key::W))
			position += movementSpeed * front;
		else if (hyp::Input::isKeyPressed(hyp::Key::S))
			position -= movementSpeed * front;
		else if (hyp::Input::isKeyPressed(hyp::Key::A))
			position -= glm::normalize(glm::cross(front, up)) * movementSpeed;
		else if (hyp::Input::isKeyPressed(hyp::Key::D))
			position += glm::normalize(glm::cross(front, up)) * movementSpeed;

		return true;
	}

	bool onMouseMoved(hyp::MouseMovedEvent& event) {
		float xpos = event.getX();
		float ypos = event.getY();

		static float lastX = xpos, lastY = ypos;
		if (!hideCursor)
		{
			return false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;

		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		update();

		return true;
	}

public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	bool hideCursor = false;

	void update() {
		glm::vec3 newFront {};
		newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		newFront.y = sin(glm::radians(pitch));
		newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(newFront);

		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
};