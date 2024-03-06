#include "shader.hpp"
#include "utils.hpp"
#include "utils/logger.hpp"
#include <fstream>

using namespace hyp;

Shader::Shader(SHADER_TYPE type) : id(generateUUID())
{
  this->shader_type = type;
  this->shader = glCreateShader(type);
}

Shader::~Shader()
{
  glDeleteShader(shader);
}

bool Shader::compile(std::string shaderSource)
{
  if (m_isCompiled)
  {
    return false;
  }

  const char *source = shaderSource.c_str();
  glShaderSource(this->shader, 1, &source, nullptr);
  glCompileShader(this->shader);

  this->m_isCompiled = true;
  return true;
}

bool Shader::operator==(const Shader &shader)
{
  if (shader.id == this->id)
  {
    return true;
  }
  return false;
}

ShaderProgram::ShaderProgram()
{
  this->m_program = glCreateProgram();
}

hyp::ShaderProgram::ShaderProgram(const std::string &vertexPath, const std::string &fragmentPath)
{
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

  hyp::Shader vshader = Shader(hyp::SHADER_TYPE::VERTEX);
  vshader.compile(vertexCode);

  hyp::Shader fshader = Shader(hyp::SHADER_TYPE::FRAGMENT);
  fshader.compile(fragmentCode);

  int success;
  char infoLog[512];

  glGetShaderiv(vshader.shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vshader.shader, 512, NULL, infoLog);
    HYP_ERROR("Vertex Shader Compilation Failed\nError:\n", infoLog);
  }

  glGetShaderiv(fshader.shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fshader.shader, 512, NULL, infoLog);
    HYP_ERROR("Fragment Shader Compilation Failed\nError:\n", infoLog);
  }

  this->attachShader(vshader);
  this->attachShader(fshader);
}

void ShaderProgram::attachShader(Shader &shader)
{
  if (m_isLinked)
  {
    HYP_WARN("Shader program is already linked. Cannot attach shader.");
    return;
  }

  unsigned int rawShader = shader.getShader();

  for (const auto &attachedShader : m_attachedShaders)
  {
    if (attachedShader->getShader() == rawShader)
    {
      HYP_WARN("Shader is already attached to the program.");
      return;
    }
  }

  glAttachShader(m_program, rawShader);

  m_attachedShaders.push_back(CreateRef<Shader>(shader));
}

void ShaderProgram::link()
{
  if (this->m_isLinked)
  {
    return;
  }
  glLinkProgram(this->m_program);
  this->m_isLinked = true;
}

void ShaderProgram::use()
{
  glUseProgram(this->m_program);
}
