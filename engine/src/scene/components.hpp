#pragma once
#ifndef HYP_COMPONENTS_HPP
	#define HYP_COMPONENTS_HPP

	#include <glm/glm.hpp>
	#include <renderer/texture.hpp>
	#include <string>

namespace hyp {

	struct TagComponent
	{
		std::string name;
	};

	struct TransformComponent
	{
		glm::vec3 position;
		glm::vec2 size;
		float rotation;

		TransformComponent() {
			position = { 0.f, 0.f, 0.f };
			rotation = 0.f;
			size = { 0.f, 0.f };
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 color;
		hyp::Ref<hyp::Texture2D> texture;
		float tilingFactor = 1.f;

		SpriteRendererComponent(const glm::vec4& color = glm::vec4(1.0))
		    : color(color) {}
	};

	struct CircleRendererComponent
	{
		glm::vec4 color;
		float thickness; // 0.1 - 1.0
		float fade;      // 0.1 - 1.0

		CircleRendererComponent(const glm::vec4& color = glm::vec4(1.0))
		    : color(color) {
			thickness = 1.f;
			fade = 0.f;
		}
	};

} // namespace hyp

#endif