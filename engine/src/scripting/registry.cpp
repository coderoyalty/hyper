#include "registry.hpp"
#include "script_engine.hpp"
#undef UNDEF_SOL
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <glm/glm.hpp>
namespace hyp {
	namespace scripting {
		extern sol::state* getState();
		hyp::Entity getEntityByName(const std::string& name);
	}

	void ScriptRegistry::register_all() {
		auto state = scripting::getState();
		register_maths(state);
		register_utils(state);
		register_components(state);
	}

	void ScriptRegistry::register_maths(sol::state* lua) {
		// vec2
		lua->new_usertype<glm::vec2>(
		    "vec2",
		    sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
		    "x", &glm::vec2::x,
		    "y", &glm::vec2::y,
		    sol::meta_function::multiplication, sol::overload(sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator*)),
		    sol::meta_function::addition, sol::overload(sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator+)),
		    sol::meta_function::subtraction, sol::overload(sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator-)),
		    sol::meta_function::division, sol::overload(sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator/)));

		// vec3
		lua->new_usertype<glm::vec3>(
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
		lua->new_usertype<glm::vec4>(
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

	void ScriptRegistry::register_utils(sol::state* lua) {
		lua->set_function("require", [&](const std::string& module)
		{
			//TODO: dynamically fetch asset path
			hyp::ScriptEngine::run_script("assets/scripts/" + module);
		});

		lua->new_enum("Component",
		    "None", utils::ComponentType::None,
		    "Transform", utils::ComponentType::Transform,
		    "Sprite", utils::ComponentType::Sprite,
		    "Circle", utils::ComponentType::Circle);
	}

	void ScriptRegistry::register_components(sol::state* lua) {
		// transform
		lua->new_usertype<hyp::TransformComponent>(
		    "TransformComponent",
		    sol::constructors<hyp::TransformComponent()>(),
		    "position", (&hyp::TransformComponent::position),
		    "rotation", (&hyp::TransformComponent::rotation),
		    "scale", (&hyp::TransformComponent::scale));

		// sprite
		lua->new_usertype<hyp::SpriteRendererComponent>(
		    "SpriteRendererComponent",
		    sol::constructors<hyp::SpriteRendererComponent(), hyp::SpriteRendererComponent(glm::vec4)>(),
		    "color", &hyp::SpriteRendererComponent::color,
		    "tilingFactor", &hyp::SpriteRendererComponent::tilingFactor);

		// circle
		lua->new_usertype<hyp::CircleRendererComponent>(
		    "CircleRendererComponent",
		    sol::constructors<hyp::CircleRendererComponent(), hyp::CircleRendererComponent(glm::vec4)>(),
		    "color", (&hyp::CircleRendererComponent::color),
		    "fade", &hyp::CircleRendererComponent::fade,
		    "thickness", &hyp::CircleRendererComponent::thickness);
	}
}