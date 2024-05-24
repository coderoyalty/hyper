#include <pch.h>
#include <scripting/script_engine.hpp>
#include <scripting/registry.hpp>
#include "entt_registry.hpp"
#include "hyp_registry.hpp"
#include <io/input.hpp>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#if defined(HYPER_DEBUG)
	#define SCRIPT_TRACE(message, ...) HYP_TRACE("[ScriptEngine] "##message, ##__VA_ARGS__)
	#define SCRIPT_ERROR(message, ...) HYP_ERROR("[ScriptEngine] "##message, ##__VA_ARGS__)
#else // error message is important!
	#include <stdio.h>
	#define SCRIPT_TRACE(message, ...)
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

	static void append_enum_values(sol::table& table, const std::initializer_list<std::pair<std::string, hyp::Key>>& values) {
		for (const auto& pair : values)
		{
			table[pair.first] = pair.second;
		}
	}

	static sol::table open_hyper(sol::this_state s) {
		sol::state_view lua { s };

		sol::table& hyper_module = lua["hyper"].get_or_create<sol::table>();

		hyper_module.set_function("find_entity", find_entity);
		hyper_module.set_function("create_entity", create_entity);
		hyper_module.set_function("destroy_entity", sol::overload(destroy_entity, destroy_entity_name));

		hyper_module.set_function("keyPressed", hyp::Input::isKeyPressed);
		hyper_module.set_function("mousePressed", hyp::Input::isMouseBtnPressed);

		return hyper_module;
	}

#define AUTO_ARG(x) decltype(x), x

	void ScriptEngine::init() {
		SCRIPT_TRACE("Initializing");

		register_meta_component<hyp::TextComponent>();
		register_meta_component<hyp::IDComponent>();
		register_meta_component<hyp::TagComponent>();
		register_meta_component<hyp::TransformComponent>();
		register_meta_component<hyp::SpriteRendererComponent>();
		register_meta_component<hyp::CircleRendererComponent>();
		register_meta_component<hyp::RigidBodyComponent>();
		register_meta_component<hyp::CircleColliderComponent>();
		register_meta_component<hyp::BoxColliderComponent>();

		auto& lua = scripting::getState();

		ScriptRegistry::register_all();

		lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package, sol::lib::string);
		lua.require("registry", sol::c_call<AUTO_ARG(&open_registry)>, false);
		lua.require("hyper", sol::c_call<AUTO_ARG(&open_hyper)>, false);

		std::string scriptsPath = "resources/hyper/lib/";

		if (fs::exists(scriptsPath))
		{
			lua["package"]["path"] = lua["package"]["path"].get<std::string>() + ";" + scriptsPath + "?.lua";
		}
	}

	sol::load_result ScriptEngine::load_script(const std::string& file) {
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

	bool ScriptEngine::addScriptComponent(hyp::Entity& entity, const std::string& scriptPath, bool update_if_exist) {
		if (!fs::exists(scriptPath)) return false;

		if (entity.has<hyp::ScriptComponent>() && !update_if_exist) return false;

		auto path = fs::relative(scriptPath).string();
		auto& script = entity.add<hyp::ScriptComponent>();
		script.script_file = path;
		return true;
	}

	void ScriptEngine::onCreateEntity(entt::registry& registry, entt::entity entity) {
		auto& script = registry.get<hyp::ScriptComponent>(entity);

		auto loaded_result = load_script(script.script_file);
		script.self = loaded_result.call();

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

	void ScriptEngine::onDestroyEntity(entt::registry& registry, entt::entity entity) {
		auto& script = registry.get<ScriptComponent>(entity);
		if (auto&& f = script.self["destroy"]; f.valid()) f(script.self);
		script.self.abandon();
	}

	void ScriptEngine::onUpdateEntities(entt::registry& registry, float dt) {
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
