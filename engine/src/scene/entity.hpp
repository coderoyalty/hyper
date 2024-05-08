#pragma once
#ifndef HYP_ENTITY_HPP
	#define HYP_ENTITY_HPP

	#include <core/application.hpp>
	#include <scene/scene.hpp>

namespace hyp {

	class Entity {
	public:
		Entity() = default;
		Entity(const Entity&) = default;

		Entity(entt::entity handle, Scene* scene);

		// add component to the entity
		template <typename T, typename... Args>
		T& add(Args&&... args) {
			HYP_ASSERT_CORE(!has<T>(), "entity already has component");
			T& component = m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);

			return component;
		}

		// [Debug]
		// for safely adding a component to the entity without getting an assertion error
		template<typename T, typename... Args>
		T& getOrAdd(Args&... args) {
			if (has<T>()) {
				return get<T>();
			}

			return add<T>(std::forward<Args>(args)...);
		}

		template <typename T>
		bool has() { return m_scene->m_registry.any_of<T>(m_handle); }

		template <typename T>
		T& get() {
			HYP_ASSERT_CORE(has<T>(), "entity does not have component");
			return m_scene->m_registry.get<T>(m_handle);
		}

		template <typename T>
		void remove() {
			HYP_ASSERT_CORE(has<T>(), "entity does not have component");
			m_scene->m_registry.remove<T>(m_handle);
		}

		operator entt::entity() const { return m_handle; }
		bool operator==(const Entity& other) const { return this->m_handle == other.m_handle; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

		operator bool() { return m_handle != entt::null; }

	private:
		entt::entity m_handle { entt::null };
		Scene* m_scene;
	};

} // namespace hyp

#endif