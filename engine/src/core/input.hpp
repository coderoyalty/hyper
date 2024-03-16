#ifndef HYPER_INPUT_HPP
#define HYPER_INPUT_HPP

#include <core/key_code.hpp>
#include <core/application.hpp>
#include <glm/glm.hpp>

namespace hyp {
	class Input {
	public:
		static bool isKeyPressed(KeyCode key);

		static glm::vec2 getMousePos();

		static float getMouseX();
		static float getMouseY();
	};
}

#endif // !HYPER_INPUT_HPP
