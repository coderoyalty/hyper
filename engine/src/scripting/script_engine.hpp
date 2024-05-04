#pragma once
#ifndef HYP_SCRIPT_ENGINE_HPP
	#define HYP_SCRIPT_ENGINE_HPP

	#include <scene/scene.hpp>
	#include <scene/entity.hpp>
	#include <scene/components.hpp>
	#include <string>

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
		static void deinit();

		static void run_script(const std::string& file);

		static void load_script(const std::string& file);

		static void onCreateEntity(hyp::Entity entity);
		static void onDestroyEntity(hyp::Entity entity);
		static void onUpdateEntity(hyp::Entity entity, float dt);
		static void onUpdateEntity(const hyp::Ref<hyp::Scene>& scene, float dt);
	};
}

#endif