#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include "core/base.hpp"
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace hyp {

	enum SHADER_TYPE
	{
		FRAGMENT = GL_FRAGMENT_SHADER,
		VERTEX
	};

	class ShaderProgram {
	public:
		ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
		ShaderProgram();
		~ShaderProgram();

		static hyp::Ref<ShaderProgram> create(const std::string& vertexPath, const std::string& fragmentPath);

	public:
		void link();
		void use();

		void setInt(const std::string& name, int value);
		void setFloat(const std::string& name, float value);
		void setBool(const std::string& name, bool value) {
			setInt(name, (int)value);
		}

		void setVec2(const std::string& name, const glm::vec2& value);
		void setVec3(const std::string& name, const glm::vec3& value);
		void setVec4(const std::string& name, const glm::vec4& value);

		void setMat2(const std::string& name, const glm::mat2& value);
		void setMat3(const std::string& name, const glm::mat3& value);
		void setMat4(const std::string& name, const glm::mat4& value);

		void setBlockBinding(const std::string& name, uint32_t bindingIndex);

	private:
		void attachShader(uint32_t& shader);
		int getLocation(const std::string& name);
		int getBlockIndex(const std::string& name);

	private:
		std::unordered_map<std::string, int> m_locations;
		bool m_isLinked = false;
		unsigned int m_program;
	};
};
#endif