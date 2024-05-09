#include <scripting/script_engine.hpp>
#include <scripting/registry.hpp>
#include <utils/logger.hpp>
#include "entt_registry.hpp"
#include <fstream>

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

	bool ScriptEngine::add_script(hyp::Entity& entity, const std::string& scriptPath, bool update_if_exist) {
		if (!fs::exists(scriptPath)) return false;

		if (entity.has<hyp::ScriptComponent>() && !update_if_exist) return false;

		auto path = fs::relative(scriptPath).string();
		auto result = load_script(path);
		auto& script = entity.add<hyp::ScriptComponent>(result.call());
		script.script_file = path;

		return true;
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

bool hyp::scripting::create_script_file(const std::string& filePath, std::string entityName) {
	std::string templatePath = "assets/template/script.lua";
	if (fs::exists(filePath) || !fs::exists(templatePath))
	{
		HYP_ERROR("could not open %s or %s because it doesn't exist", filePath.c_str(), templatePath.c_str());
		return false;
	}

	if (entityName.empty())
	{
		HYP_WARN("entity name is empty, resetting to a default name");
		entityName = "node";
	}

	std::ifstream inFile(templatePath);
	std::ofstream outFile(filePath);

	if (inFile.bad() || outFile.bad())
	{
		HYP_ERROR("issue with opening script template file or output file");
		return false;
	}

	auto replace_str = [](std::string& str, std::string searchStr, std::string replacement)
	{
		size_t found = str.find(searchStr);
		if (found == std::string::npos) return;
		str.replace(found, searchStr.length(), replacement);
	};

	std::string line;

	while (std::getline(inFile, line))
	{
		replace_str(line, "EntityName", entityName);
		outFile << line << std::endl;
	}

	inFile.close();
	outFile.close();

	return true;
}
