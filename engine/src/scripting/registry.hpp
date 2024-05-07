#pragma once

#ifndef HYP_SCRIPTING_REGISTRY_HPP
	#define HYP_SCRIPTING_REGISTRY_HPP

#include <string>
namespace sol {
	class state;
}

namespace hyp {
	class ScriptRegistry {
	public:
		static void register_all();
	private:
		// exposes gl maths related program entities
		static void register_maths(sol::state& state);
		// exposes utility-based program entities
		static void register_utils(sol::state& state);
		//
		static void register_components(sol::state& state);
	};
}

#endif
