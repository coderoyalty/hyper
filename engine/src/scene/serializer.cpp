#include "serializer.hpp"
#include <scene/entity.hpp>
#include <scene/components.hpp>
#include <fstream>
#include <scripting/script_engine.hpp>

namespace YAML {

	template <>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs) {
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template <>
	struct convert<hyp::UUID>
	{
		static Node encode(const hyp::UUID& uuid) {
			Node node;
			node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, hyp::UUID& uuid) {
			uuid = node.as<uint64_t>();
			return true;
		}
	};
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

namespace utils {
	template <typename T>
	void map_value(YAML::Emitter& emitter, const std::string& key, const T& value) {
		emitter << YAML::Key << key << YAML::Value << value;
	}

	static std::string serializeRigidBodyType(hyp::RigidBodyComponent::BodyType type) {
		switch (type)
		{
		case hyp::RigidBodyComponent::BodyType::Static:
			return "Static";
		case hyp::RigidBodyComponent::BodyType::Dynamic:
			return "Dynamic";
		case hyp::RigidBodyComponent::BodyType::Kinematic:
			return "Kinematic";
		default:
			break;
		}

		return "Static";
	};

	static hyp::RigidBodyComponent::BodyType deserializeRigidBodyType(const std::string& name) {
		if (name == "Static") return hyp::RigidBodyComponent::BodyType::Static;
		if (name == "Dynamic") return hyp::RigidBodyComponent::BodyType::Dynamic;
		if (name == "Kinematic") return hyp::RigidBodyComponent::BodyType::Kinematic;

		HYP_ASSERT(false);
		return hyp::RigidBodyComponent::BodyType::Static;
	}

	static void serializeEntity(YAML::Emitter& emitter, hyp::Entity entity) {
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Entity" << YAML::Value << entity.getUUID();

		{
			if (entity.has<hyp::TagComponent>())
			{
				emitter << YAML::Key << "TagComponent";
				emitter << YAML::BeginMap; // TagComponent

				auto& tag = entity.getName();
				emitter << YAML::Key << "Name" << YAML::Value << tag;

				emitter << YAML::EndMap; // TagComponent
			}

			if (entity.has<hyp::TransformComponent>())
			{
				emitter << YAML::Key << "TransformComponent";
				emitter << YAML::BeginMap; // TransformComponent

				auto& tc = entity.get<hyp::TransformComponent>();
				emitter << YAML::Key << "Position" << YAML::Value << tc.position;
				emitter << YAML::Key << "Rotation" << YAML::Value << tc.rotation;
				emitter << YAML::Key << "Scale" << YAML::Value << tc.scale;

				emitter << YAML::EndMap; // TransformComponent
			}

			if (entity.has<hyp::SpriteRendererComponent>())
			{
				emitter << YAML::Key << "SpriteRendererComponent";
				emitter << YAML::BeginMap; // SpriteRendererComponent

				auto& sc = entity.get<hyp::SpriteRendererComponent>();
				emitter << YAML::Key << "Color" << YAML::Value << sc.color;
				if (sc.texture)
				{
					emitter << YAML::Key << "Texture" << YAML::Value << sc.texture->getPath();
				}
				emitter << YAML::Key << "TilingFactor" << YAML::Value << sc.tilingFactor;

				emitter << YAML::EndMap; // SpriteRendererComponent
			}

			if (entity.has<hyp::CircleRendererComponent>())
			{
				emitter << YAML::Key << "CircleRendererComponent";
				emitter << YAML::BeginMap; // CircleRendererComponent
				auto& cc = entity.get<hyp::CircleRendererComponent>();

				emitter << YAML::Key << "Color" << YAML::Value << cc.color;
				emitter << YAML::Key << "Fade" << YAML::Value << cc.fade;
				emitter << YAML::Key << "Thickness" << YAML::Value << cc.thickness;

				emitter << YAML::EndMap; // CircleRendererComponent
			}

			if (entity.has<hyp::TextComponent>())
			{
				emitter << YAML::Key << "TextComponent";

				emitter << YAML::BeginMap; // TextComponent

				auto& tc = entity.get<hyp::TextComponent>();

				emitter << YAML::Key << "Color" << YAML::Value << tc.color;
				emitter << YAML::Key << "Text" << YAML::Value << tc.text;
				emitter << YAML::Key << "FontSize" << YAML::Value << tc.fontSize;
				emitter << YAML::Key << "LineSpacing" << YAML::Value << tc.lineSpacing;

				//TODO: line-spacing is not really taking effect... this is from the batch renderer territory..
				//		font asset serialization not handled...

				emitter << YAML::EndMap; // TextComponent
			}

			if (entity.has<hyp::ScriptComponent>())
			{
				emitter << YAML::Key << "ScriptComponent";
				emitter << YAML::BeginMap; // ScriptComponent

				auto& script = entity.get<hyp::ScriptComponent>();
				emitter << YAML::Key << "Path" << YAML::Value << script.script_file;

				emitter << YAML::EndMap; // ScriptComponent
			}

			if (entity.has<hyp::RigidBodyComponent>())
			{
				emitter << YAML::Key << "RigidBodyComponent";
				emitter << YAML::BeginMap; // RigidBodyComponent

				const auto& rigidBody = entity.get<hyp::RigidBodyComponent>();

				emitter << YAML::Key << "Type" << YAML::Value << serializeRigidBodyType(rigidBody.type);

				emitter << YAML::Key << "FixedRotation" << YAML::Value << rigidBody.fixedRotation;

				emitter << YAML::EndMap;
			}

			if (entity.has<hyp::BoxColliderComponent>())
			{
				emitter << YAML::Key << "BoxColliderComponent";
				emitter << YAML::BeginMap;

				const auto& bc = entity.get<hyp::BoxColliderComponent>();

				emitter << YAML::Key << "Offset" << YAML::Value << bc.offset;
				emitter << YAML::Key << "Size" << YAML::Value << bc.size;
				emitter << YAML::Key << "Density" << YAML::Value << bc.density;
				emitter << YAML::Key << "Friction" << YAML::Value << bc.friction;
				emitter << YAML::Key << "Restitution" << YAML::Value << bc.restitution;
				emitter << YAML::Key << "RestitutionThreshold" << YAML::Value << bc.restitutionThreshold;

				emitter << YAML::EndMap;
			}

			if (entity.has<hyp::CircleColliderComponent>())
			{
				emitter << YAML::Key << "CircleColliderComponent";
				emitter << YAML::BeginMap;

				const auto& bc = entity.get<hyp::CircleColliderComponent>();

				emitter << YAML::Key << "Offset" << YAML::Value << bc.offset;
				emitter << YAML::Key << "Radius" << YAML::Value << bc.radius;
				emitter << YAML::Key << "Density" << YAML::Value << bc.density;
				emitter << YAML::Key << "Friction" << YAML::Value << bc.friction;
				emitter << YAML::Key << "Restitution" << YAML::Value << bc.restitution;
				emitter << YAML::Key << "RestitutionThreshold" << YAML::Value << bc.restitutionThreshold;

				emitter << YAML::EndMap;
			}
		}

		emitter << YAML::EndMap;
	}
}

hyp::SceneSerializer::SceneSerializer(const hyp::Ref<hyp::Scene>& scene) {
	m_scene = scene;
}

void hyp::SceneSerializer::serializer(const std::string& path) {
	YAML::Emitter emitter;

	emitter << YAML::Value;

	emitter << YAML::BeginMap;
	{
		emitter << YAML::Key << "Scene" << YAML::Value << "Untitled-Hyper Scene";
		emitter << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		// must only serialize entities with a tag & transform component
		auto view = m_scene->m_registry.view<hyp::TagComponent, hyp::TransformComponent>();
		for (auto entityId : view)
		{
			hyp::Entity entity { entityId, m_scene.get() };
			if (!entity) return;

			utils::serializeEntity(emitter, entity);
		}

		emitter << YAML::EndSeq;
	}
	emitter << YAML::EndMap;

	std::ofstream fout(path, std::ios::binary);

	fout.write(emitter.c_str(), emitter.size());
}

bool hyp::SceneSerializer::deserializer(const std::string& path) {
	YAML::Node node;

	try
	{
		node = YAML::LoadFile(path);
	}
	catch (YAML::ParserException e)
	{
		HYP_FATAL("Failed to load scene file %s\n\t%s", path.c_str(), e.what());
		return false;
	}

	if (!node["Scene"])
	{
		HYP_ERROR("Loaded data is not a scene file");
		return false;
	};

	std::string sceneName = node["Scene"].as<std::string>();

	HYP_TRACE("Deserializing Scene %s", sceneName.c_str());

	auto entities = node["Entities"];

	if (entities)
	{
		for (auto entity : entities)
		{
			uint64_t uuid = {};

			if (entity["Entity"])
			{
				uuid = entity["Entity"].as<uint64_t>();
			}
			else
			{
				uuid = UUID();
			}

			std::string name;
			auto tagComponent = entity["TagComponent"];
			if (tagComponent)
			{
				name = tagComponent["Name"].as<std::string>();
			}

			HYP_TRACE("Deserialized entity with name = %s", name.c_str());

			hyp::Entity deserializedEntity = m_scene->createEntity(UUID(uuid), name);

			auto transformComponent = entity["TransformComponent"];
			if (transformComponent)
			{
				auto& tc = deserializedEntity.getOrAdd<hyp::TransformComponent>();
				tc.position = transformComponent["Position"].as<glm::vec3>();
				tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
				tc.scale = transformComponent["Scale"].as<glm::vec3>();
			}

			auto spriteComponent = entity["SpriteRendererComponent"];
			if (spriteComponent)
			{
				auto& sc = deserializedEntity.add<hyp::SpriteRendererComponent>();
				sc.color = spriteComponent["Color"].as<glm::vec4>();

				if (spriteComponent["TilingFactor"])
				{
					sc.tilingFactor = spriteComponent["TilingFactor"].as<float>();
				}

				if (spriteComponent["Texture"])
				{
					auto texturePath = spriteComponent["Texture"].as<std::string>();
					sc.texture = hyp::Texture2D::create(texturePath);
				}
			}

			auto circleComponent = entity["CircleRendererComponent"];
			if (circleComponent)
			{
				auto& cc = deserializedEntity.add<hyp::CircleRendererComponent>();

				cc.color = circleComponent["Color"].as<glm::vec4>();
				cc.fade = circleComponent["Fade"].as<float>();
				cc.thickness = circleComponent["Thickness"].as<float>();
			}

			auto textComponent = entity["TextComponent"];
			if (textComponent)
			{
				auto& tc = deserializedEntity.add<hyp::TextComponent>();
				tc.color = textComponent["Color"].as<glm::vec4>();
				tc.fontSize = textComponent["FontSize"].as<float>();
				tc.lineSpacing = textComponent["LineSpacing"].as<float>();
				tc.text = textComponent["Text"].as<std::string>();
			}

			auto scriptComponent = entity["ScriptComponent"];
			if (scriptComponent)
			{
				auto file = scriptComponent["Path"].as<std::string>();
				if (!fs::exists(file))
				{
					HYP_WARN("%s does not exist, skipping this operation", file.c_str());
					continue;
				}
				auto script = hyp::ScriptEngine::load_script(file);
				auto& sc = deserializedEntity.add<hyp::ScriptComponent>(script.call());
				sc.script_file = file;
			}

			auto rigidBodyComponent = entity["RigidBodyComponent"];
			if (rigidBodyComponent)
			{
				auto& rb = deserializedEntity.add<hyp::RigidBodyComponent>();
				if (rigidBodyComponent["FixedRotation"])
				{
					rb.fixedRotation = rigidBodyComponent["FixedRotation"].as<bool>();
				}
				rb.type = utils::deserializeRigidBodyType(rigidBodyComponent["Type"].as<std::string>());
			}

			auto boxCollider = entity["BoxColliderComponent"];
			if (boxCollider)
			{
				auto& bc = deserializedEntity.add<hyp::BoxColliderComponent>();
				bc.density = boxCollider["Density"].as<float>();
				bc.offset = boxCollider["Offset"].as<glm::vec2>();
				bc.size = boxCollider["Size"].as<glm::vec2>();
				bc.restitutionThreshold = boxCollider["RestitutionThreshold"].as<float>();
				bc.restitution = boxCollider["Restitution"].as<float>();
				bc.friction = boxCollider["Friction"].as<float>();
			}

			auto circleCollider = entity["CircleColliderComponent"];
			if (circleCollider)
			{
				auto& cc = deserializedEntity.add<hyp::CircleColliderComponent>();
				cc.density = circleCollider["Density"].as<float>();
				cc.offset = circleCollider["Offset"].as<glm::vec2>();
				cc.radius = circleCollider["Radius"].as<float>();
				cc.restitutionThreshold = circleCollider["RestitutionThreshold"].as<float>();
				cc.restitution = circleCollider["Restitution"].as<float>();
				cc.friction = circleCollider["Friction"].as<float>();
			}
		}
	}
	return true;
}
