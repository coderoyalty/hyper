#include "shader.hpp"
#include "utils.hpp"
#include "utils/logger.hpp"

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

void ShaderProgram::attachShader(Shader &shader)
{
  if (m_isLinked)
  {
    std::cout << "Error: Shader program is already linked. Cannot attach shader.\n";
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
