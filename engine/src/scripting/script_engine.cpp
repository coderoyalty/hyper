#include <scripting/script_engine.hpp>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace hyp {
	struct ScriptEngineData
	{
		sol::state* lua_state;
	};

	static ScriptEngineData s_data;
}