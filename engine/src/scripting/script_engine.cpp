#include <scripting/script_engine.hpp>
#include <scripting/registry.hpp>
#include <utils/logger.hpp>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "lua.h"
#include "lstate.h"
#include "setjmp.h"

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
		sol::state* lua_state;
		std::map<std::string, hyp::Entity*> m_entityMap;
	};

	static ScriptEngineData s_data;

	static jmp_buf s_luaPanicJump;

	static int lua_AtPanicJump(lua_State* lua) {
		const char* message = lua_tostring(lua, -1);
		if (message)
			SCRIPT_ERROR("%s", message);
		longjmp(s_luaPanicJump, 1);
		return 0; // will never return
	}

	namespace scripting {
		extern sol::state* getState() {
			HYP_ASSERT_CORE(s_data.lua_state != nullptr, "ScriptEngine not initialized");
			return s_data.lua_state;
		}

		extern hyp::Entity& getEntityByName(const std::string& name) {
			/*HYP_ASSERT_CORE(s_data.lua_state != nullptr, "ScriptEngine not initialized");
			if (s_data.m_entityMap.find(name) != s_data.m_entityMap.end())
			{
				return s_data.m_entityMap[name];
			};*/

			return hyp::Entity();
		}
	}

	void ScriptEngine::init() {
		HYP_ASSERT_CORE(s_data.lua_state == nullptr, "ScriptEngine already initialized before calling %s", __func__);
		SCRIPT_TRACE("Initializing");
		s_data.lua_state = new sol::state();
		s_data.lua_state->open_libraries(sol::lib::base, sol::lib::math);

		auto L = s_data.lua_state->lua_state();

		L->l_G->panic = [](lua_State* L)
		{
			std::cout << "[ScriptEngine] PANIC!!! We should never reach this line!\n";
			return 0;
		};

		lua_atpanic(L, &lua_AtPanicJump);

		ScriptRegistry::register_all();
	}

	void ScriptEngine::deinit() {
		HYP_ASSERT_CORE(s_data.lua_state != nullptr, "ScriptEngine not initialized before calling %s", __func__);
		SCRIPT_TRACE("Shutting down");
		delete s_data.lua_state;
		s_data.lua_state = nullptr;
	}

	void ScriptEngine::run_script(const std::string& file) {
		SCRIPT_TRACE("Running %s...", file.c_str());
		try
		{
			s_data.lua_state->script_file(file);
		}
		catch (const sol::error& e)
		{
			SCRIPT_ERROR("%s", e.what());
		}
	}

	void ScriptEngine::load_script(const std::string& file) {
		SCRIPT_TRACE("Loading script: %s", file.c_str());

		auto load_result = s_data.lua_state->load_file(file);

		if (!load_result.valid())
		{
			sol::error e = load_result;
			SCRIPT_ERROR("Failed to load script '%s': %s", file.c_str(), e.what());
		}
		else
		{
			SCRIPT_TRACE("Script loaded successfully: %s", file.c_str());

			auto func_result = load_result();
			if (!func_result.valid())
			{
				sol::error e = func_result;
				SCRIPT_ERROR("Failed to execute script '%s': %s", file.c_str(), e.what());
			}
			else
			{
				SCRIPT_TRACE("Script executed successfully: %s", file.c_str());
			}
		}
	}

#define LUA_CALL(Namespace, Function, ...)     \
	if (setjmp(s_luaPanicJump) == 0)           \
	{                                          \
		lua[Namespace][Function](__VA_ARGS__); \
	}                                          \
	else                                       \
	{                                          \
		SCRIPT_ERROR("Lua internal error!");   \
		return;                                \
	}

	void ScriptEngine::onCreateEntity(hyp::Entity entity) {
		auto& lua = *s_data.lua_state;
		if (!entity || !entity.has<hyp::ScriptComponent>()) return;

		auto& sc = entity.get<hyp::ScriptComponent>();
		LUA_CALL(sc.className, "onCreate");
	}

	void ScriptEngine::onDestroyEntity(hyp::Entity entity) {
		auto& lua = *s_data.lua_state;
		if (!entity || !entity.has<hyp::ScriptComponent>()) return;

		auto& sc = entity.get<hyp::ScriptComponent>();
		LUA_CALL(sc.className, "onDestroy");
	}

	void ScriptEngine::onUpdateEntity(hyp::Entity entity, float dt) {
		auto& lua = *s_data.lua_state;
		if (!entity || !entity.has<hyp::ScriptComponent>()) return;

		auto& sc = entity.get<hyp::ScriptComponent>();
		LUA_CALL(sc.className, "onUpdate", dt);
	}

	void ScriptEngine::onUpdateEntity(const hyp::Ref<hyp::Scene>& scene, float dt) {
		scene->m_registry.each([&](entt::entity entity)
		{
			onUpdateEntity({ entity, scene.get() }, dt);
		});
	}
}