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

void hyp::Scene::destroyEntity(Entity entity) {
	m_registry.destroy(entity);
}

void hyp::Scene::onUpdate(float dt) {
	{
		auto& sprite_group = m_registry.group<TransformComponent, hyp::SpriteRendererComponent>();

		for (auto& entity : sprite_group)
		{
			auto& [transform, sprite] = sprite_group.get<TransformComponent, hyp::SpriteRendererComponent>(entity);
			hyp::Renderer2D::drawQuad(transform.position, transform.size, sprite.texture, sprite.tilingFactor, sprite.color, (int)entity);
		}
	}

{
		auto& circle_group = m_registry.view<TransformComponent, hyp::CircleRendererComponent>();

		for (auto& entity : circle_group)
		{
			auto& [transform, circle] = circle_group.get<TransformComponent, hyp::CircleRendererComponent>(entity);

			glm::mat4 model(1.0);

			model = glm::translate(model, transform.position);
			model = glm::scale(model, glm::vec3(transform.size, 0.f));

			hyp::Renderer2D::drawCircle(model, circle.thickness, circle.fade, circle.color, (int)entity);
		}
	}
}
