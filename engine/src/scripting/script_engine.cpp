#include <scripting/script_engine.hpp>
#include <scripting/registry.hpp>
#include <utils/logger.hpp>
#include "entt_registry.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


#if defined(HYPER_DEBUG)
	#define SCRIPT_TRACE(message, ...) HYP_TRACE("[ScriptEngine] "##message, ##__VA_ARGS__)
	#define SCRIPT_ERROR(message, ...) HYP_ERROR("[ScriptEngine] "##message, ##__VA_ARGS__)
#else // error message is important!
	#include <stdio.h>
	#define SCRIPT_ERROR(message, ...) printf(message, ##__VA_ARGS__);
#endif

namespace hyp {
	struct ScriptEngineData
	{
		sol::state lua_state {};
	};

	static ScriptEngineData s_data;


	namespace scripting {
		extern sol::state& getState() {
			return s_data.lua_state;
		}
	}

#define AUTO_ARG(x) decltype(x), x

	void ScriptEngine::init() {
		SCRIPT_TRACE("Initializing");

		register_meta_component<hyp::TransformComponent>();
		register_meta_component<hyp::SpriteRendererComponent>();
		register_meta_component<hyp::CircleRendererComponent>();

		auto& lua = scripting::getState();

		lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package, sol::lib::string);
		lua.require("registry", sol::c_call<AUTO_ARG(&open_registry)>, false);

		ScriptRegistry::register_all();
	}

	sol::load_result ScriptEngine::load_script(const std::string& file) {
		SCRIPT_TRACE("Loading script: %s", file.c_str());

		auto load_result = s_data.lua_state.load_file(file);

		if (!load_result.valid())
		{
			sol::error e = load_result;
			SCRIPT_ERROR("Failed to load script '%s': %s", file.c_str(), e.what());
		}
		else
		{
			SCRIPT_TRACE("Script loaded successfully: %s", file.c_str());
		}
		return load_result;
	}

	void ScriptEngine::init_script(entt::registry& registry, entt::entity entity) {
		auto& script = registry.get<hyp::ScriptComponent>(entity);
		HYP_ASSERT(script.self.valid());
		script.hooks.update = script.self["update"];
		HYP_ASSERT(script.hooks.update.valid());

		script.self["id"] = sol::readonly_property([entity]
		{
			return entity;
		});
		script.self["owner"] = std::ref(registry);
		if (auto&& f = script.self["init"]; f.valid()) f(script.self);
	}

	void ScriptEngine::free_script(entt::registry& registry, entt::entity entity) {
		auto& script = registry.get<ScriptComponent>(entity);
		if (auto&& f = script.self["destroy"]; f.valid()) f(script.self);
		script.self.abandon();
	}

	void ScriptEngine::update_script(entt::registry& registry, float dt) {
		auto& view = registry.view<hyp::ScriptComponent>();

		for (auto entity : view)
		{
			auto& script = registry.get<hyp::ScriptComponent>(entity);
			assert(script.self.valid());
			script.hooks.update(script.self, dt);
		}
	}
}