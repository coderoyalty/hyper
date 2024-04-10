#pragma once
#ifndef HYP_COMPONENTS_HPP
#define HYP_COMPONENTS_HPP

#include <glm/glm.hpp>
#include <string>

namespace hyp {

struct TagComponent {
	std::string name;
};

struct TransformComponent {
	glm::vec3 position;
	glm::vec2 size;
	float rotation;

	TransformComponent() {
		position = {0.f, 0.f, 0.f};
		rotation = 0.f;
		size = {0.f, 0.f};
	}
};

struct SpriteComponent {
	glm::vec4 color;

	SpriteComponent() { color = glm::vec4(1.0); }
};

} // namespace hyp

#endif