#include "input.hpp"
#include <core/application.hpp>

bool hyp::Input::isKeyPressed(hyp::Key key) {
	GLFWwindow* window = hyp::Application::get().getWindow()->getNativeWindow();
	int state = glfwGetKey(window, (int)key);

	return state == GLFW_PRESS;
}

glm::vec2 hyp::Input::getMousePos() {
	GLFWwindow* window = hyp::Application::get().getWindow()->getNativeWindow();
	double xpos = 0.f, ypos = 0.f;
	glfwGetCursorPos(window, &xpos, &ypos);

	return glm::vec2((float)xpos, (float)ypos);
}

float hyp::Input::getMouseX() {
	return getMousePos().x;
}

float hyp::Input::getMouseY() {
	return getMousePos().y;
}
