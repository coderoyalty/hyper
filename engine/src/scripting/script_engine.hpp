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

	namespace scripting {
		bool create_script_file(const std::string& filePath, std::string entityName);
	}

	class ScriptEngine {
	public:
		static void init();

		static sol::load_result load_script(const std::string& file);

		/// @brief
		/// load a script and add a script component to an entity.
		/// Note: the script file in the component is relative to the editor's location
		///
		static bool add_script(hyp::Entity& entity, const std::string& scriptPath, bool update_if_exist = false);

		static void init_script(entt::registry& registry, entt::entity entity);
		static void free_script(entt::registry& registry, entt::entity entity);
		static void update_script(entt::registry& registry, float dt);
	};
}

#endif