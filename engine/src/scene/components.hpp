#pragma once
#ifndef HYP_COMPONENTS_HPP
	#define HYP_COMPONENTS_HPP

	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <renderer/texture.hpp>
	#include <renderer/font.hpp>
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

		glm::mat4 getTransform() {
			glm::mat4 model(1.0);
			model = glm::translate(model, position);
			model = glm::scale(model, glm::vec3(size, 0.f));
			model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));

			return model;
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

	struct TextComponent
	{
		std::string text;
		hyp::Ref<hyp::Font> font;
		glm::vec4 color { 1.f };
		float lineSpacing = 0.f;
		float fontSize = 16.f;
	};

} // namespace hyp

#endif