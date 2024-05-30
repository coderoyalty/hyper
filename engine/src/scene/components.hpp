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
	#include "scene_camera.hpp"

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

	struct CameraComponent
	{
		hyp::SceneCamera camera;
		bool primary = true;
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
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
		bool failed = false;

		struct
		{
			sol::protected_function update;
		} hooks;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};

	struct RigidBodyComponent
	{
		enum class BodyType
		{
			Static = 0, // immovable, can't collide with other static bodies.
			Dynamic,    // movable and affected by forces
			Kinematic,  // manually controlled and not affected by forces. like a Static | Dynamic :)
		};
		BodyType type = BodyType::Static;
		bool fixedRotation = false;

		// runtime storage for the physics engine
		void* runtime = nullptr;
	};

	struct BoxColliderComponent
	{
		glm::vec2 offset = { 0.f, 0.f };
		glm::vec2 size = { 0.5f, 0.5f }; // 0.5 by default because box2d works well with half width, height.

		float density = 1.f;
		float friction = 0.5f;
		float restitution = 0.f;
		float restitutionThreshold = 0.5f;
	};

	struct CircleColliderComponent
	{
		glm::vec2 offset = { 0.0f, 0.0f };
		float radius = 0.5f;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
	};

	template <typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents =
	    ComponentGroup<TransformComponent, CameraComponent,
	        SpriteRendererComponent, CircleRendererComponent, ScriptComponent, TextComponent,
	        RigidBodyComponent, BoxColliderComponent, CircleColliderComponent>;

} // namespace hyp

#endif