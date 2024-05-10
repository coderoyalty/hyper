#pragma once
#ifndef HYP_COMPONENTS_HPP
	#define HYP_COMPONENTS_HPP

	#include <renderer/texture.hpp>
	#include <renderer/font.hpp>
	#include <string>

	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>

	#include <sol/sol.hpp>

	#include <core/uuid.hpp>

namespace hyp {

	struct IDComponent
	{
		hyp::UUID id;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string name;
	};

	struct TransformComponent
	{
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;

		TransformComponent() {
			position = { 0.f, 0.f, 0.f };
			rotation = { 0.f, 0.f, 0.f };
			scale = { 1.f, 1.f, 1.f };
		}

		glm::mat4 getTransform() const;
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

	struct ScriptComponent
	{
		sol::table self;
		std::string script_file; //TODO:

		struct
		{
			sol::function init;
			sol::function update;
			sol::function destroy;
		} hooks;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};

} // namespace hyp

#endif