#pragma once
#ifndef HYPER_REGISTRY_HPP
	#define HYPER_REGISTRY_HPP

	#include <entt/entt.hpp>
	#include <entt/entity/registry.hpp>
	#include <entt/entity/runtime_view.hpp>
	#include <scene/components.hpp>

entt::entity find_entity(entt::registry& reg, const std::string& name) {
	auto view = reg.view<hyp::TagComponent>();
	for (auto entity : view)
	{
		const auto& tag = view.get<hyp::TagComponent>(entity);
		if (tag.name == name)
			return entity;
	}

	return entt::null;
};

entt::entity create_entity(entt::registry& reg, const std::string& name) {
	auto entity = find_entity(reg, name);
	if (reg.valid(entity))
	{
		return entity;
	}

	entity = reg.create();
	auto& tag = reg.emplace<hyp::TagComponent>(entity);
	tag.name = name.empty() ? "Entity" : name;
	reg.emplace<hyp::IDComponent>(entity, hyp::UUID());
	reg.emplace<hyp::TransformComponent>(entity);

	return entity;
}

bool destroy_entity(entt::registry& reg, const entt::entity entity) {
	if (reg.valid(entity))
	{
		reg.destroy(entity);
		return true;
	}
	return false;
}

bool destroy_entity_name(entt::registry& reg, const std::string& name) {
	auto entity = find_entity(reg, name);
	return destroy_entity(reg, entity);
}

#endif