#include "shader.hpp"
#include "utils/logger.hpp"
#include <fstream>

using namespace hyp;

namespace Helpers {
	static bool compileShader(uint32_t& shader, const std::string& content, const SHADER_TYPE& shaderType) {
		shader = glCreateShader(shaderType);
		const char* source = content.c_str();
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		int32_t compileStatus;

		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

		if (!compileStatus)
		{
			// Compilation failed
			GLint infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::vector<GLchar> infoLog(infoLogLength);
			glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog.data());
			// Print or handle the error
			switch (shaderType)
			{
			case VERTEX:
			{
				HYP_ERROR("Vertex Shader Compilation Failed\nError:\n");
				HYP_ERROR(std::string(infoLog.data(), infoLog.size()));
				break;
			}
			case FRAGMENT:
			{
				HYP_ERROR("Fragment Shader Compilation Failed\nError:\n");
				HYP_ERROR(std::string(infoLog.data(), infoLog.size()));

				break;
			}
			default:
				break;
			}

			return false;
		}

		return true;
	}
}

ShaderProgram::ShaderProgram() : m_isLinked(false) {
	this->m_program = glCreateProgram();
}

hyp::ShaderProgram::~ShaderProgram() {
	glDeleteProgram(m_program);
}

hyp::Ref<ShaderProgram> hyp::ShaderProgram::create(const std::string& vertexPath, const std::string& fragmentPath) {
	return hyp::CreateRef<ShaderProgram>(vertexPath, fragmentPath);
}

hyp::ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
	this->m_program = glCreateProgram();

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream fragmentFile;
	std::ifstream vertexFile;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);

		if (!vertexFile.is_open() || !fragmentFile.is_open())
		{
			HYP_ERROR("Failed to open shader files");
			return;
		}

		vertexFile.seekg(0, std::ios::end);
		std::streampos vertexFileSize = vertexFile.tellg();
		vertexFile.seekg(0, std::ios::beg);

		if (vertexFileSize == 0)
		{
			HYP_ERROR("Vertex Shader File is Empty");
			return;
		}

		fragmentFile.seekg(0, std::ios::end);
		std::streampos fragmentFileSize = fragmentFile.tellg();
		fragmentFile.seekg(0, std::ios::beg);

		if (fragmentFileSize == 0)
		{
			HYP_ERROR("Fragment Shader File is Empty");
			return;
		}

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vertexFile.rdbuf();
		fShaderStream << fragmentFile.rdbuf();
		fragmentFile.close();
		vertexFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		HYP_ERROR("Shader file not successfully read");
		return;
	};

	uint32_t vshader, fshader;

	bool vshaderStatus = Helpers::compileShader(vshader, vertexCode, SHADER_TYPE::VERTEX);
	bool fshaderStatus = Helpers::compileShader(fshader, fragmentCode, SHADER_TYPE::FRAGMENT);

	if (!vshaderStatus || !fshaderStatus)
	{
		HYP_WARN("Didn't create a shader program because there was a compilation error");
		return;
	}

	this->attachShader(vshader);
	this->attachShader(fshader);
}

void hyp::ShaderProgram::attachShader(uint32_t& shader) {
	if (m_isLinked)
	{
		HYP_WARN("Shader program is already linked. Cannot attach shader.");
		return;
	}

	glAttachShader(m_program, shader);
}

int hyp::ShaderProgram::getLocation(const std::string& name) {
	auto it = m_locations.find(name);

	if (it != m_locations.end())
	{
		return it->second;
	}

	int32_t location = glGetUniformLocation(this->m_program, name.c_str());

	m_locations.insert({ name, location });
	return location;
}

int hyp::ShaderProgram::getBlockIndex(const std::string& name) {
	auto it = m_locations.find(name);

	if (it != m_locations.end())
	{
		return it->second;
	}

	int32_t location = glGetUniformBlockIndex(m_program, name.c_str());

	m_locations.insert({ name, location });
	return location;
}

void ShaderProgram::link() {
	if (this->m_isLinked)
	{
		return;
	}
	glLinkProgram(this->m_program);
	this->m_isLinked = true;
}

void ShaderProgram::use() {
	glUseProgram(this->m_program);
}

void hyp::ShaderProgram::setInt(const std::string& name, int value) {
	int location = this->getLocation(name);
	glUniform1i(location, value);
}

void hyp::ShaderProgram::setFloat(const std::string& name, float value) {
	int location = this->getLocation(name);
	glUniform1f(location, value);
}

void hyp::ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) {
	int location = this->getLocation(name);
	glUniform2f(location, value.x, value.y);
}

void hyp::ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) {
	int location = this->getLocation(name);
	glUniform3f(location, value.x, value.y, value.z);
}

void hyp::ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) {
	int location = this->getLocation(name);
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void hyp::ShaderProgram::setMat2(const std::string& name, const glm::mat2& value) {
	int location = this->getLocation(name);
	glUniformMatrix2fv(location, 1, false, glm::value_ptr(value));
}

void hyp::ShaderProgram::setMat3(const std::string& name, const glm::mat3& value) {
	int location = this->getLocation(name);
	glUniformMatrix3fv(location, 1, false, glm::value_ptr(value));
}

void hyp::ShaderProgram::setMat4(const std::string& name, const glm::mat4& value) {
	int location = this->getLocation(name);
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
}

void hyp::ShaderProgram::setBlockBinding(const std::string& name, uint32_t blockBinding) {
	glUniformBlockBinding(m_program, getBlockIndex(name), blockBinding);
}
