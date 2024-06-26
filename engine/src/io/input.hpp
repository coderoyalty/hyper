#ifndef HYPER_INPUT_HPP
#define HYPER_INPUT_HPP

#include <io/key_code.hpp>
#include <io/mouse_code.hpp>
#include <glm/glm.hpp>

namespace hyp {
	class Input {
	public:
		static bool isKeyPressed(hyp::Key key);

		static bool isMouseBtnPressed(hyp::Mouse mouse);

		static glm::vec2 getMousePos();

		/*
		* get mouse position relative to the window position
		*/
		static glm::vec2 getMouseAbsPos();

		static float getMouseX();
		static float getMouseY();
	};
}

#endif // !HYPER_INPUT_HPP
