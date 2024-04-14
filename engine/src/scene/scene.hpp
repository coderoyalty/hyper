#pragma once
#ifndef HYP_SCENE_HPP
	#define HYP_SCENE_HPP

	#include <entt.hpp>
	#include <string>

namespace hyp {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name);

		void onUpdate(float dt);

	private:
		friend class Entity;
		entt::registry m_registry;
	};
} // namespace hyp

#endif