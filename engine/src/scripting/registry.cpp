#include "pch.h"
#include "registry.hpp"
#include "script_engine.hpp"
#undef UNDEF_SOL
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <glm/glm.hpp>

#include <box2d/box2d.h>

namespace hyp {
	namespace scripting {
		extern sol::state& getState();
	}

	namespace {
		struct PhysicsBody
		{
			b2Body* body = nullptr;

			PhysicsBody(entt::registry* registry, entt::entity entity) {
				HYP_ASSERT(registry);
				if (!registry->valid(entity))
				{
					body = nullptr;
				}

				if (registry->any_of<hyp::RigidBodyComponent>(entity))
				{
					auto& rbc = registry->get<hyp::RigidBodyComponent>(entity);

					this->body = (b2Body*)rbc.runtime;
				}
			}

			void applyImpulseToCenter(const glm::vec2& impulse, bool wake = true) {
				if (!body) return;

				body->ApplyLinearImpulseToCenter(b2Vec2 { impulse.x, impulse.y }, wake);
			}

			void applyForceToCenter(const glm::vec2& force, bool wake = true) {
				if (!body) return;

				body->ApplyForceToCenter(b2Vec2 { force.x, force.y }, wake);
			}

			void applyTorque(float torque, bool wake = true) {
				if (!body) return;

				body->ApplyTorque(torque, wake);
			}

			bool isAwake() const {
				if (!body) return false;
				return body->IsAwake();
			}
		};
	}

	void ScriptRegistry::register_all() {
		auto& state = scripting::getState();
		register_maths(state);
		register_utils(state);
		register_components(state);
	}

	void ScriptRegistry::register_maths(sol::state& lua) {
		// vec2
		lua.new_usertype<glm::vec2>(
		    "vec2",
		    sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
		    "x", &glm::vec2::x,
		    "y", &glm::vec2::y,
		    sol::meta_function::multiplication, sol::overload(sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator*)),
		    sol::meta_function::addition, sol::overload(sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator+)),
		    sol::meta_function::subtraction, sol::overload(sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator-)),
		    sol::meta_function::division, sol::overload(sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator/)));

		// vec3
		lua.new_usertype<glm::vec3>(
		    "vec3",
		    sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
		    "x", &glm::vec3::x,
		    "y", &glm::vec3::y,
		    "z", &glm::vec3::z,
		    sol::meta_function::multiplication, sol::overload(sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator*)),
		    sol::meta_function::addition, sol::overload(sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator+)),
		    sol::meta_function::subtraction, sol::overload(sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator-)),
		    sol::meta_function::division, sol::overload(sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator/)));

		// vec4
		lua.new_usertype<glm::vec4>(
		    "vec4",
		    sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>(),
		    "x", &glm::vec4::x,
		    "y", &glm::vec4::y,
		    "z", &glm::vec4::z,
		    "w", &glm::vec4::w,
		    sol::meta_function::multiplication, sol::overload(sol::resolve<glm::vec4(const glm::vec4&, const glm::vec4&)>(glm::operator*)),
		    sol::meta_function::addition, sol::overload(sol::resolve<glm::vec4(const glm::vec4&, const glm::vec4&)>(glm::operator+)),
		    sol::meta_function::subtraction, sol::overload(sol::resolve<glm::vec4(const glm::vec4&, const glm::vec4&)>(glm::operator-)),
		    sol::meta_function::division, sol::overload(sol::resolve<glm::vec4(const glm::vec4&, const glm::vec4&)>(glm::operator/)));
	}

	//Hmmm?
	void ScriptRegistry::register_utils(sol::state& lua) {
		lua.new_usertype<hyp::PhysicsBody>("PhysicsBody",
		    sol::constructors<hyp::PhysicsBody(entt::registry * registry, entt::entity entity)>(),
		    "applyImpulseToCenter", &hyp::PhysicsBody::applyImpulseToCenter,
		    "applyForceToCenter", &hyp::PhysicsBody::applyForceToCenter,
		    "applyTorque", &hyp::PhysicsBody::applyTorque,
		    "isAwake", &hyp::PhysicsBody::isAwake);

		auto& module = lua["hyper"].get_or_create<sol::table>();
	}

	void ScriptRegistry::register_components(sol::state& lua) {
		lua.new_usertype<hyp::UUID>("UUID", sol::constructors<hyp::UUID(), hyp::UUID(uint64_t)>(),
		    "operator", &hyp::UUID::operator size_t);
		// id component

		lua.new_usertype<hyp::IDComponent>(
		    "IDComponent", sol::constructors<hyp::IDComponent()>(),
		    "type_id", &entt::type_hash<hyp::IDComponent>::value,
		    "id", &hyp::IDComponent::id);

		lua.new_usertype<hyp::TagComponent>(
		    "TagComponent", sol::constructors<hyp::TagComponent()>(),
		    "type_id", &entt::type_hash<hyp::TagComponent>::value,
		    "name", &hyp::TagComponent::name);

		// transform
		lua.new_usertype<hyp::TransformComponent>(
		    "Transform",
		    sol::constructors<hyp::TransformComponent()>(),
		    "type_id", &entt::type_hash<hyp::TransformComponent>::value,
		    "position", (&hyp::TransformComponent::position),
		    "rotation", (&hyp::TransformComponent::rotation),
		    "scale", (&hyp::TransformComponent::scale));

		// sprite
		lua.new_usertype<hyp::SpriteRendererComponent>(
		    "SpriteComponent",
		    sol::constructors<hyp::SpriteRendererComponent(), hyp::SpriteRendererComponent(glm::vec4)>(),
		    "type_id", &entt::type_hash<hyp::SpriteRendererComponent>::value,
		    "color", &hyp::SpriteRendererComponent::color,
		    "tilingFactor", &hyp::SpriteRendererComponent::tilingFactor);

		// circle
		lua.new_usertype<hyp::CircleRendererComponent>(
		    "CircleComponent",
		    sol::constructors<hyp::CircleRendererComponent(), hyp::CircleRendererComponent(glm::vec4)>(),
		    "type_id", &entt::type_hash<hyp::CircleRendererComponent>::value,
		    "color", (&hyp::CircleRendererComponent::color),
		    "fade", &hyp::CircleRendererComponent::fade,
		    "thickness", &hyp::CircleRendererComponent::thickness);

		// circle
		lua.new_usertype<hyp::TextComponent>(
		    "TextComponent",
		    sol::constructors<hyp::TextComponent()>(),
		    "type_id", &entt::type_hash<hyp::TextComponent>::value,
		    "text", (&hyp::TextComponent::text),
		    "color", (&hyp::TextComponent::color),
		    "lineSpacing", &hyp::TextComponent::lineSpacing,
		    "fontSize", &hyp::TextComponent::fontSize);

		lua.new_enum<hyp::RigidBodyComponent::BodyType>("BodyType",
		    {
		        { "Static", hyp::RigidBodyComponent::BodyType::Static },
		        { "Dynamic", hyp::RigidBodyComponent::BodyType::Dynamic },
		        { "Kinematic", hyp::RigidBodyComponent::BodyType::Kinematic },
		    });

		lua.new_usertype<hyp::RigidBodyComponent>(
		    "RigidBodyComponent",
		    sol::constructors<hyp::RigidBodyComponent()>(),
		    "type_id", &entt::type_hash<hyp::RigidBodyComponent>::value,
		    "fixedRotation", (&hyp::RigidBodyComponent::fixedRotation),
		    "type", (&hyp::RigidBodyComponent::type));

		lua.new_usertype<hyp::BoxColliderComponent>("BoxColliderComponent",
		    sol::constructors<hyp::BoxColliderComponent()>(),
		    "type_id", &entt::type_hash<hyp::BoxColliderComponent>::value,
		    "density", (&hyp::BoxColliderComponent::density),
		    "restitution", (&hyp::BoxColliderComponent::restitution),
		    "friction", (&hyp::BoxColliderComponent::friction),
		    "restitutionThreshold", (&hyp::BoxColliderComponent::restitutionThreshold),
		    "offset", (&hyp::BoxColliderComponent::offset),
		    "size", (&hyp::BoxColliderComponent::size));

		lua.new_usertype<hyp::CircleColliderComponent>("CircleColliderComponent",
		    sol::constructors<hyp::CircleColliderComponent()>(),
		    "type_id", &entt::type_hash<hyp::CircleColliderComponent>::value,
		    "density", (&hyp::CircleColliderComponent::density),
		    "restitution", (&hyp::CircleColliderComponent::restitution),
		    "friction", (&hyp::CircleColliderComponent::friction),
		    "restitutionThreshold", (&hyp::CircleColliderComponent::restitutionThreshold),
		    "offset", (&hyp::CircleColliderComponent::offset),
		    "radius", (&hyp::CircleColliderComponent::radius));
	}
}
