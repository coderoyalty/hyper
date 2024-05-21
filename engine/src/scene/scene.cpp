#include "scene.hpp"
#include "renderer/renderer2d.hpp"
#include "scene/components.hpp"
#include "scene/entity.hpp"
#include "scripting/script_engine.hpp"
#include <box2d/box2d.h>

namespace utils {
	static b2BodyType rigidBodyToBox2D(hyp::RigidBodyComponent::BodyType bodyType) {
		switch (bodyType)
		{
		case hyp::RigidBodyComponent::BodyType::Static:
			return b2_staticBody;
		case hyp::RigidBodyComponent::BodyType::Dynamic:
			return b2_dynamicBody;
		case hyp::RigidBodyComponent::BodyType::Kinematic:
			return b2_kinematicBody;
		default:
			return b2_staticBody;
		}
	}
};

template <typename... Component>
static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<hyp::UUID, entt::entity>& enttMap) {
	([&]()
	{
		auto view = src.view<Component>();
		for (auto srcEntity : view)
		{
			entt::entity dstEntity = enttMap.at(src.get<hyp::IDComponent>(srcEntity).id);

			auto& srcComponent = src.get<Component>(srcEntity);
			dst.emplace_or_replace<Component>(dstEntity, srcComponent);
		}
	}(),
	    ...);
}

template <typename... Component>
static void CopyComponent(hyp::ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<hyp::UUID, entt::entity>& enttMap) {
	CopyComponent<Component...>(dst, src, enttMap);
}

template <typename... Component>
static void CopyComponentIfExists(hyp::Entity dst, hyp::Entity src) {
	([&]()
	{
		if (src.has<Component>())
			dst.addOrReplace<Component>(src.get<Component>());
	}(),
	    ...);
}

template <typename... Component>
static void CopyComponentIfExists(hyp::ComponentGroup<Component...>, hyp::Entity dst, hyp::Entity src) {
	CopyComponentIfExists<Component...>(dst, src);
}

hyp::Ref<hyp::Scene> hyp::Scene::copy(hyp::Ref<hyp::Scene> other) {
	auto& newScene = CreateRef<hyp::Scene>();

	auto& srcRegistry = other->m_registry;
	auto& dstRegistry = newScene->m_registry;

	std::unordered_map<UUID, entt::entity> enttMap;

	auto idView = srcRegistry.view<IDComponent>();
	for (auto e : idView)
	{
		UUID uuid = srcRegistry.get<IDComponent>(e).id;
		const auto& name = srcRegistry.get<TagComponent>(e).name;
		Entity newEntity = newScene->createEntity(uuid, name);
		enttMap[uuid] = (entt::entity)newEntity;
	}

	CopyComponent(AllComponents {}, dstRegistry, srcRegistry, enttMap);

	return newScene;
}

hyp::Entity hyp::Scene::createEntity(const std::string& name) {
	return createEntity(UUID(), name);
}

hyp::Entity hyp::Scene::createEntity(hyp::UUID uuid, const std::string& name) {
	entt::entity handle = m_registry.create();
	Entity entity = Entity(handle, this);
	auto& id = entity.add<IDComponent>(uuid);
	auto& tag = entity.add<TagComponent>();
	tag.name = name.empty() ? "Entity" : name;
	auto& transform = entity.add<TransformComponent>();

	m_entityMap[uuid] = handle;
	return entity;
}

void hyp::Scene::destroyEntity(Entity entity) {
	m_entityMap.erase(entity.getUUID());
	m_registry.destroy(entity);
}

hyp::Entity hyp::Scene::duplicateEntity(Entity entity) {
	std::string name = entity.getName();
	Entity newEntity = createEntity(name);

	CopyComponentIfExists(AllComponents {}, newEntity, entity);
	return newEntity;
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
			model = glm::scale(model, transform.scale);

			hyp::Renderer2D::drawCircle(transform.getTransform(), circle.thickness, circle.fade, circle.color, (int)entity);
		}
	}

	{
		auto text_group = m_registry.view<TransformComponent, TextComponent>();

		for (auto& entity : text_group)
		{
			auto& [transform, text] = text_group.get<TransformComponent, TextComponent>(entity);

			glm::mat4 model(1.f);

			model = glm::translate(model, transform.position);
			model = glm::scale(model, transform.scale);

			hyp::Renderer2D::TextParams textParams;
			textParams.fontSize = text.fontSize;
			textParams.leading = text.lineSpacing;
			textParams.color = text.color;
			hyp::Renderer2D::drawString(text.text, text.font, transform.getTransform(), textParams, (int)entity);
		}
	}
}

void hyp::Scene::onRuntimeStart() {
	onPhysicsStart();
	{
		m_running = true;
		auto& view = m_registry.view<hyp::ScriptComponent>();

		for (auto e : view)
		{
			hyp::ScriptEngine::onCreateEntity(m_registry, e);
		}
	}
}

void hyp::Scene::onRuntimeStop() {
	onPhysicsStop();
	{
		m_running = false;
		auto& view = m_registry.view<hyp::ScriptComponent>();

		for (auto e : view)
		{
			hyp::ScriptEngine::onDestroyEntity(m_registry, e);
		}
	}
}

void hyp::Scene::onUpdateRuntime(float dt) {
	if (m_paused) { return; }

	hyp::ScriptEngine::onUpdateEntities(m_registry, dt);
	{
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;

		auto view = getEntities<RigidBodyComponent>();

		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.get<TransformComponent>();
			auto& rb2d = entity.get<RigidBodyComponent>();

			b2Body* body = (b2Body*)rb2d.runtime;

			if (!body) continue;

			const auto& position = transform.position;
			const auto& rotation = transform.rotation.z;

			const auto& body_pos = body->GetPosition();

			// Check if position or rotation has changed
			if ((position.x != body_pos.x || position.y != body_pos.y) || rotation != body->GetAngle())
			{
				body->SetTransform(b2Vec2 { position.x, position.y }, rotation);

				body->SetAwake(true);
			}
		}

		m_physicsWorld->Step(dt, velocityIterations, positionIterations);

		// Retrieve transform from Box2D
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.get<TransformComponent>();
			auto& rb2d = entity.get<RigidBodyComponent>();

			b2Body* body = (b2Body*)rb2d.runtime;

			if (!body) continue;

			const auto& position = body->GetPosition();
			transform.position.x = position.x;
			transform.position.y = position.y;
			transform.rotation.z = body->GetAngle();
		}
	}
}

void hyp::Scene::onPhysicsStart() {
	m_physicsWorld = new b2World({ 0.f, -9.8f });

	auto view = getEntities<hyp::RigidBodyComponent>();
	for (auto e : view)
	{
		Entity entity = { e, this };

		auto& transform = entity.get<hyp::TransformComponent>();
		auto& rigidBody = entity.get<hyp::RigidBodyComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = utils::rigidBodyToBox2D(rigidBody.type);
		bodyDef.position.Set(transform.position.x, transform.position.y);
		bodyDef.angle = transform.rotation.z;

		b2Body* body = m_physicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rigidBody.fixedRotation);
		rigidBody.runtime = body; // store the body so it can be used during runtime update

		if (entity.has<hyp::BoxColliderComponent>())
		{
			auto& bc = entity.get<hyp::BoxColliderComponent>();
			b2PolygonShape shape;

			shape.SetAsBox(bc.size.x * transform.scale.x, bc.size.y * transform.scale.y, b2Vec2(bc.offset.x, bc.offset.y), 0.f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = bc.density;
			fixtureDef.friction = bc.friction;
			fixtureDef.restitution = bc.restitution;
			fixtureDef.restitutionThreshold = bc.restitutionThreshold;

			b2Fixture* fixture = body->CreateFixture(&fixtureDef);
		}

		if (entity.has<hyp::CircleColliderComponent>())
		{
			auto& cc = entity.get<hyp::CircleColliderComponent>();

			b2CircleShape shape;

			shape.m_p.Set(cc.offset.x, cc.offset.y);
			shape.m_radius = transform.scale.x * cc.radius;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = cc.density;
			fixtureDef.friction = cc.friction;
			fixtureDef.restitution = cc.restitution;
			fixtureDef.restitutionThreshold = cc.restitutionThreshold;

			b2Fixture* fixture = body->CreateFixture(&fixtureDef);
		}
	}
}

void hyp::Scene::onPhysicsStop() {
	delete m_physicsWorld;
	m_physicsWorld = nullptr;
}

hyp::Entity hyp::Scene::findEntity(const std::string& tagName) {
	auto view = getEntities<TagComponent>();
	for (auto entity : view)
	{
		const auto& tag = view.get<TagComponent>(entity);
		if (tag.name == tagName)
			return Entity { entity, this };
	}
	return Entity();
}

hyp::Entity hyp::Scene::getEntity(UUID uuid) {
	if (m_entityMap.find(uuid) != m_entityMap.end())
		return Entity { m_entityMap[uuid], this };

	return Entity();
}
