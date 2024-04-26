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
			hyp::Renderer2D::drawQuad(transform.getTransform(), sprite.texture, sprite.tilingFactor, sprite.color, (int)entity);
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

			hyp::Renderer2D::drawCircle(transform.getTransform(), circle.thickness, circle.fade, circle.color, (int)entity);
		}
	}

	{
		auto text_group = m_registry.view<TransformComponent, TextComponent>();

		for (auto& entity : text_group)
		{
			auto& [transform, text] = text_group.get<TransformComponent, TextComponent>(entity);

			glm::mat4 model(1.f);

			model = glm::translate(model, transform.position + glm::vec3(transform.size * 0.5f, 0.f));
			model = glm::scale(model, glm::vec3(transform.size, 0.f));

			hyp::Renderer2D::TextParams textParams;
			textParams.fontSize = text.fontSize;
			textParams.leading = text.lineSpacing;
			textParams.color = text.color;
			hyp::Renderer2D::drawString(text.text, text.font, transform.getTransform(), textParams, (int)entity);
		}
	}
}
