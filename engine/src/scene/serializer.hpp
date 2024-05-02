#pragma once
#ifndef HYP_SCENE_SERIALIZER_HPP

	#define HYP_SCENE_SERIALIZER_HPP

	#include <core/base.hpp>
	#include <scene/scene.hpp>
	#include <yaml-cpp/yaml.h>

namespace hyp {
	class SceneSerializer {
	public:
		SceneSerializer(const hyp::Ref<hyp::Scene>& scene);

		void serializer(const std::string& path);
		bool deserializer(const std::string& path);

	private:
		hyp::Ref<hyp::Scene> m_scene;
	};
}

#endif
