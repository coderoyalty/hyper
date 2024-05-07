#pragma once
#ifndef HYP_SCRIPT_ENGINE_HPP
	#define HYP_SCRIPT_ENGINE_HPP

	#include <scene/scene.hpp>
	#include <scene/entity.hpp>
	#include <scene/components.hpp>
	#include <string>
	#include <entt/entt.hpp>

namespace utils {

	enum ComponentType
	{
		None,
		Transform,
		Sprite,
		Circle
	};
}

namespace hyp {
	class ScriptEngine {
	public:
		static void init();

		static sol::load_result load_script(const std::string& file);

		static void init_script(entt::registry& registry, entt::entity entity);
		static void free_script(entt::registry& registry, entt::entity entity);
		static void update_script(entt::registry& registry, float dt);
	};
}

#endif