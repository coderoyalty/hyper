#include "scene.hpp"
#include "renderer/renderer2d.hpp"
#include "scene/components.hpp"
#include "scene/entity.hpp"

hyp::Scene::Scene() {}

hyp::Scene::~Scene() {}

hyp::Entity hyp::Scene::createEntity(const std::string& name) {
	entt::entity handle = m_registry.create();

	Entity entity = Entity(handle, this);

	auto& tag = entity.add<TagComponent>();
	tag.name = name.empty() ? "Entity" : name;
	auto& transform = entity.add<TransformComponent>();

	return entity;
}

void hyp::Scene::onUpdate(float dt) {
	auto& view = m_registry.group<TransformComponent>(entt::get<hyp::SpriteRendererComponent>);

	for (auto entity : view)
	{
		auto& [transform, sprite] = view.get<TransformComponent, hyp::SpriteRendererComponent>(entity);
		hyp::Renderer2D::drawQuad(transform.position, transform.size, sprite.color);
	}
}
