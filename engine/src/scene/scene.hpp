#pragma once
#ifndef HYP_SCENE_HPP
	#define HYP_SCENE_HPP

	#include <entt/entt.hpp>
	#include <string>
	#include <core/uuid.hpp>
	#include <core/base.hpp>
	#include <unordered_map>
	#include <glm/glm.hpp>

class b2World;

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

		Entity duplicateEntity(Entity entity);

		void onUpdate(const glm::mat4& viewProjection);

		void onRuntimeStart();
		void onRuntimeStop();

		void onUpdateRuntime(float dt);

		void onViewportSize(uint32_t width, uint32_t height);

		Entity findEntity(const std::string& tagName);
		Entity getEntity(UUID uuid);

		void setPause(bool value) {
			m_paused = value;
		}
		bool isPaused() const {
			return m_paused;
		};
		bool isRunning() const {
			return m_running;
		}

		void onPhysicsStart();
		void onPhysicsStop();

	private:
		void renderScene(const glm::mat4& viewProjection);

	private:
		bool m_running = false;
		bool m_paused = false;

		uint32_t m_vwidth = 0;
		uint32_t m_vheight = 0;

		b2World* m_physicsWorld = nullptr;

		entt::registry m_registry;
		std::unordered_map<hyp::UUID, entt::entity> m_entityMap;
	};
} // namespace hyp

#endif
