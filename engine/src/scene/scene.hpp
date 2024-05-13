#pragma once
#ifndef HYP_SCENE_HPP
	#define HYP_SCENE_HPP

	#include <entt/entt.hpp>
	#include <string>
	#include <core/uuid.hpp>
	#include <core/base.hpp>
	#include <unordered_map>

namespace hyp {
	class Entity;

	class Scene {
		friend class Entity;
		friend class HierarchyPanel;
		friend class SceneSerializer;
		friend class ScriptEngine;

	public:
		Scene() = default;
		~Scene() = default;

		static hyp::Ref<Scene> copy(hyp::Ref<Scene> other);

		template <typename... Components>
		auto getEntities() {
			return m_registry.view<Components...>();
		}

	public:
		Entity createEntity(const std::string& name);
		Entity createEntity(hyp::UUID uuid, const std::string& name);
		void destroyEntity(Entity entity);

		void onUpdate(float dt);

		void onRuntimeStart();
		void onRuntimeStop();

		void onUpdateRuntime(float dt);

		Entity findEntity(const std::string& tagName);
		Entity getEntity(UUID uuid);

	private:
		bool m_running = false;
		bool m_paused = false;

		entt::registry m_registry;
		std::unordered_map<hyp::UUID, entt::entity> m_entityMap;
	};
} // namespace hyp

#endif