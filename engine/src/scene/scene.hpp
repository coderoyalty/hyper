#pragma once
#ifndef HYP_SCENE_HPP
	#define HYP_SCENE_HPP

	#include <entt.hpp>
	#include <string>

namespace hyp {
	class Entity;

	class Scene {
		friend class Entity;
		friend class HierarchyPanel;

	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name);

		void destroyEntity(Entity entity);

		void onUpdate(float dt);

		template <typename... Components>
		auto getEntities() {
			return m_registry.view<Components...>();
		}

	private:
		entt::registry m_registry;
	};
} // namespace hyp

#endif