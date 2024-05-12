#pragma once
#ifndef HYP_SCRIPT_ENGINE_HPP
	#define HYP_SCRIPT_ENGINE_HPP

	#include <scene/scene.hpp>
	#include <scene/entity.hpp>
	#include <scene/components.hpp>
	#include <string>
	#include <entt/entt.hpp>

namespace hyp {

	namespace scripting {
		bool create_script_file(const std::string& filePath, std::string entityName);
	}

	class ScriptEngine {
	public:
		static void init();

		static sol::load_result load_script(const std::string& file);

		static bool addScriptComponent(hyp::Entity& entity, const std::string& scriptPath, bool update_if_exist = false);

		static void onCreateEntity(entt::registry& registry, entt::entity entity);
		static void onDestroyEntity(entt::registry& registry, entt::entity entity);
		static void onUpdateEntities(entt::registry& registry, float dt);
	};
}

#endif